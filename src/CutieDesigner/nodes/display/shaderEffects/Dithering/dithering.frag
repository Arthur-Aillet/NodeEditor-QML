#version 440
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;
  vec4 sourceRect;
  int mode;
  int resolution;
};
layout(binding = 1) uniform sampler2D source;


const float kern2[4*4] = float[4*4]
  (0.    ,0.5    ,0.125  ,0.625 ,
  0.75   ,0.25   ,0.875  ,0.375 ,
  0.1875 ,0.6875 ,0.0625 ,0.5625,
  0.9375 ,0.4375 ,0.8125 ,0.3125);

const float kern4[8*8] = float[8*8]
  (0.      ,0.5      ,0.125    ,0.625    ,0.03125  ,0.53125  ,0.15625  ,0.65625 ,
  0.75     ,0.25     ,0.875    ,0.375    ,0.78125  ,0.28125  ,0.90625  ,0.40625 ,
  0.1875   ,0.6875   ,0.0625   ,0.5625   ,0.21875  ,0.71875  ,0.09375  ,0.59375 ,
  0.9375   ,0.4375   ,0.8125   ,0.3125   ,0.96875  ,0.46875  ,0.84375  ,0.34375 ,
  0.046875 ,0.546875 ,0.171875 ,0.671875 ,0.015625 ,0.515625 ,0.140625 ,0.640625,
  0.796875 ,0.296875 ,0.921875 ,0.421875 ,0.765625 ,0.265625 ,0.890625 ,0.390625,
  0.234375 ,0.734375 ,0.109375 ,0.609375 ,0.203125 ,0.703125 ,0.078125 ,0.578125,
  0.984375 ,0.484375 ,0.859375 ,0.359375 ,0.953125 ,0.453125 ,0.828125 ,0.328125);

void main() {
  vec2 coord = fragCoord;
  coord.x = floor(coord.x / float(resolution));
  coord.y = floor(coord.y / float(resolution));
  vec2 uv = coord/floor(sourceRect.zw / float(resolution));
  vec4 col = texture(source, uv);

  if (mode == 0) {
    float bayerValue = kern2[((int(coord.x) % 4) * 4) + (int(coord.y) % 4)];

    fragColor = vec4(
      step(bayerValue, col.r),
      step(bayerValue, col.g),
      step(bayerValue, col.b),
    col.a);
  } else {
    float bayerValue = kern4[((int(coord.x) % 8) * 8) + (int(coord.y) % 8)];

    fragColor = vec4(
      step(bayerValue, col.r),
      step(bayerValue, col.g),
      step(bayerValue, col.b),
      col.a
    );
  }
}