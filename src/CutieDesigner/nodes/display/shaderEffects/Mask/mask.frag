#version 440
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;
  vec4 maskRect;
  vec4 imageRect;
  int mode;
  bool inverted;
  vec4 color;
  float spreadMin;
  float spreadMax;
  float tresholdMin;
  float tresholdMax;
};
layout(binding = 1) uniform sampler2D image;
layout(binding = 2) uniform sampler2D mask;

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

void main() {
  vec2 mappedCoord = fragCoord + (imageRect.xy - maskRect.xy);

  bool inside = mappedCoord.x < maskRect.z && mappedCoord.y < maskRect.w;

  vec4 colorSource = texture(image, texCoord);
  vec4 maskText = texture(mask, mappedCoord / imageRect.zw);
  float maskVal = 0;

  if (mode == 0) {
    maskVal = maskText.a;
  } else if (mode == 1) {
    maskVal = maskText.r * 0.2126 + maskText.g * 0.7152 + maskText.b * 0.0722;
  } else if (mode == 2) {
    maskVal = (maskText.r - color.r) * (maskText.r - color.r);
    maskVal += (maskText.g - color.g) * (maskText.g - color.g);
    maskVal += (maskText.b - color.b) * (maskText.b - color.b);
    maskVal = sqrt(maskVal);
  } else {
    maskVal = abs(RGBtoHSL(maskText.rgb).x - RGBtoHSL(color.rgb).x);
  }
  maskVal = min(maskVal, 1);

  const float mSLow = 1.0 + spreadMin;
  const float mSUp = 1.0 + spreadMax;
  float m1 = smoothstep(tresholdMin * mSLow - (mSLow - 0.999), tresholdMin * mSLow, maskVal);
  float m2 = smoothstep((1.0 - tresholdMax) * mSUp - (mSUp - 0.999), (1.0 - tresholdMax) * mSUp, (1.0 - maskVal));
  float mm = m1 * m2;

  maskVal = (1.0 - float(inverted)) * mm + float(inverted) * (1.0 - mm);
  fragColor = colorSource * maskVal * qt_Opacity;
}
