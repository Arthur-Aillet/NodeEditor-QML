#version 440
layout(location = 0) in vec4 qt_Vertex;
layout(location = 1) in vec2 qt_MultiTexCoord0;
layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec2 fragCoord;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;
  vec4 sourceRect;
  int mode;
  int resolution;
};

void main() {
  texCoord = qt_MultiTexCoord0;
  fragCoord = qt_Vertex.xy;
  gl_Position = qt_Matrix * qt_Vertex;
}
