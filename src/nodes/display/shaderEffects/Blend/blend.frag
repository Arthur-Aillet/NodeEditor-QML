#version 440
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;
  vec4 targetRect;
  vec4 sourceRect;
  int mode;
};
layout(binding = 1) uniform sampler2D source;
layout(binding = 2) uniform sampler2D target;

float RGBtoL(vec3 color) {
  float cmin = min(color.r, min(color.g, color.b));
  float cmax = max(color.r, max(color.g, color.b));
  float l = (cmin + cmax) / 2.0;
  return l;
}

vec3 RGBtoHSL(vec3 color) {
  float cmin = min(color.r, min(color.g, color.b));
  float cmax = max(color.r, max(color.g, color.b));
  float h = 0.0;
  float s = 0.0;
  float l = (cmin + cmax) / 2.0;
  float diff = cmax - cmin;

  if (diff > 1.0 / 256.0) {
    if (l < 0.5)
      s = diff / (cmin + cmax);
    else
      s = diff / (2.0 - (cmin + cmax));

    if (color.r == cmax)
      h = (color.g - color.b) / diff;
    else if (color.g == cmax)
      h = 2.0 + (color.b - color.r) / diff;
    else
      h = 4.0 + (color.r - color.g) / diff;

    h /= 6.0;
  }
  return vec3(h, s, l);
}

float hueToIntensity(float v1, float v2, float h) {
  h = fract(h);
  if (h < 1.0 / 6.0)
    return v1 + (v2 - v1) * 6.0 * h;
  else if (h < 1.0 / 2.0)
    return v2;
  else if (h < 2.0 / 3.0)
    return v1 + (v2 - v1) * 6.0 * (2.0 / 3.0 - h);

  return v1;
}

vec3 HSLtoRGB(vec3 color) {
  float h = color.x;
  float l = color.z;
  float s = color.y;

  if (s < 1.0 / 256.0)
    return vec3(l, l, l);

  float v1;
  float v2;
  if (l < 0.5)
    v2 = l * (1.0 + s);
  else
    v2 = (l + s) - (s * l);

  v1 = 2.0 * l - v2;

  float d = 1.0 / 3.0;
  float r = hueToIntensity(v1, v2, h + d);
  float g = hueToIntensity(v1, v2, h);
  float b = hueToIntensity(v1, v2, h - d);
  return vec3(r, g, b);
}

lowp float channelBlendHardLight(lowp float c1, lowp float c2) {
  return c2 > 0.5 ? (1.0 - (1.0 - 2.0 * (c2 - 0.5)) * (1.0 - c1)) : (2.0 * c1 * c2);
}

vec3 applyBlend(vec3 rgb1, vec3 rgb2) {
  switch (mode) {
  case 0:
    return min(rgb1 + rgb2, 1.0);
  case 1:
    return 0.5 * (rgb1 + rgb2);
  case 2:
    return HSLtoRGB(vec3(RGBtoHSL(rgb2).xy, RGBtoL(rgb1)));
  case 3:
    return clamp(1.0 - ((1.0 - rgb1) / max(vec3(1.0 / 256.0), rgb2)), vec3(0.0), vec3(1.0));
  case 4:
    return clamp(rgb1 / max(vec3(1.0 / 256.0), (1.0 - rgb2)), vec3(0.0), vec3(1.0));
  case 5:
    return min(rgb1, rgb2);
  case 6:
    return 0.3 * rgb1.r + 0.59 * rgb1.g + 0.11 * rgb1.b >
                   0.3 * rgb2.r + 0.59 * rgb2.g + 0.11 * rgb2.b
               ? rgb2
               : rgb1;
  case 7:
    return abs(rgb1 - rgb2);
  case 8:
    return clamp(rgb1 / rgb2, 0.0, 1.0);
  case 9:
    return rgb1 + rgb2 - 2.0 * rgb1 * rgb2;
  case 10:
    return vec3(channelBlendHardLight(rgb1.r, rgb2.r), channelBlendHardLight(rgb1.g, rgb2.g),
                channelBlendHardLight(rgb1.b, rgb2.b));
  case 11:
    return HSLtoRGB(vec3(RGBtoHSL(rgb2).x, RGBtoHSL(rgb1).yz));
  case 12:
    return max(rgb1, rgb2);
  case 13:
    return 0.3 * rgb1.r + 0.59 * rgb1.g + 0.11 * rgb1.b >
                   0.3 * rgb2.r + 0.59 * rgb2.g + 0.11 * rgb2.b
               ? rgb1
               : rgb2;
  case 14:
    return HSLtoRGB(vec3(RGBtoHSL(rgb1).xy, RGBtoL(rgb2)));
  case 15:
    return rgb1 * rgb2;
  case 16:
    return 1.0 - abs(1.0 - rgb1 - rgb2);
  case 17:
    vec3 hsl1 = RGBtoHSL(rgb1);
    return HSLtoRGB(vec3(hsl1.x, RGBtoHSL(rgb2).y, hsl1.z));
  case 18:
    return 1.0 - (vec3(1.0) - rgb1) * (vec3(1.0) - rgb2);
  case 19:
    return 1.0 - abs(1.0 - rgb1 - rgb2);
  case 20:
    return max(rgb1 - rgb2, vec3(0.0));
  case 21:
    return rgb1 * ((1.0 - rgb1) * rgb2 + (1.0 - (1.0 - rgb1) * (1.0 - rgb2)));
  }
}

void main() {
  vec2 mappedCoord = fragCoord + (sourceRect.xy - targetRect.xy);
  vec4 color1 = texture(target, mappedCoord / sourceRect.zw);

  bool inside = mappedCoord.x < targetRect.z && mappedCoord.y < targetRect.w;
  color1 *= (inside ? 1 : 0);

  vec4 color2 = texture(source, texCoord);

  vec3 rgb1 = color1.rgb / max(1.0 / 256.0, color1.a);
  vec3 rgb2 = color2.rgb / max(1.0 / 256.0, color2.a);
  float a = max(color1.a, color1.a * color2.a);

  // Addition
  //  fragColor = min(sourceTex + targetTex, 1.0);

  // Average
  // fragColor = 0.5 * (sourceTex + targetTex);

  // ColorBurn
  vec3 result = applyBlend(rgb1, rgb2);

  fragColor.rgb = mix(rgb1, result.rgb, color2.a);
  fragColor.rbg *= a;
  fragColor.a = a;
  fragColor = inside ? fragColor : color2;
  fragColor *= qt_Opacity;
}