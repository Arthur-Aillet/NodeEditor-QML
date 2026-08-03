#version 440
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;
  vec4 maskRect;
  vec4 imageRect;
};
layout(binding = 1) uniform sampler2D image;
layout(binding = 2) uniform sampler2D mask;

void main() {
  vec2 mappedCoord = fragCoord + (imageRect.xy - maskRect.xy);

  bool inside = mappedCoord.x < maskRect.z && mappedCoord.y < maskRect.w;

  vec4 colorSource = texture(image, texCoord);
  vec4 colorMask = texture(mask, mappedCoord / imageRect.zw);

  fragColor = colorSource * colorMask.a * qt_Opacity;
}
