#version 440
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

const float LINE_WIDTH_1 = 1.0;
const float LINE_WIDTH_2 = 3.0;
const vec3 BG_COLOR =  vec3(0.2); // Black
const vec3 LINE_COLOR =  vec3(1.0); // White

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
};

void main() {
    float N = 10.;
    float NN = N + N + 10.;
    float pix = 1.5;
    
    vec2 coord = fragCoord;
    coord -= .5;
    
    vec2 p1 = abs(mod(coord + LINE_WIDTH_1 / 2. + pix, NN) - pix);
    vec2 p2 = abs(mod(coord + LINE_WIDTH_2 / 2. + pix, 10.*NN) - pix);
    
    float g1 = min(p1.x, p1.y),
          g2 = min(p2.x ,p2.y);
    float amount = smoothstep(0.,pix, min(g1, g2));
    fragColor = vec4(mix(LINE_COLOR, BG_COLOR, amount), 1.0) * qt_Opacity;
}
