#version 440

layout(location = 0) in vec4 qt_Vertex;
layout(location = 1) in vec2 qt_MultiTexCoord0;
layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec2 fragCoord;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec3 iResolution;
    vec4 innerColor;
    vec4 outerColor;
    vec2 point1;
    vec2 point2;
    vec2 point3;
    vec2 point4;
    float k;
    float pointsScale;
    float smoothFactor;
    float fill;
    bool substraction;
    vec2 boxArea;
    float boxRadius;
    float circleScale;
    float inOutFactor;
};

out gl_PerVertex { vec4 gl_Position; };


void main() {
    texCoord = qt_MultiTexCoord0;
    fragCoord = vec2(qt_Vertex.x, iResolution.y - qt_Vertex.y);
    vec2 vertCoord = qt_Vertex.xy;
    gl_Position = qt_Matrix * vec4(vertCoord, 0.0, 1.0);
}
