#version 440
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec2 fragCoord;
layout(location = 0) out vec4 fragColor;

const float SMALL_OFFSET = 15.;
const float SMALL_WIDTH = 0.25;
const float LARGE_OFFSET = 150.;
const float LARGE_WIDTH = 0.5;
const float LARGE_WIDTH_CUT = 3.0;
const float ZOOM_CUTOFF_START = 0.8;
const float ZOOM_CUTOFF_END = 0.4;

layout(std140, binding = 0) uniform buf {
  mat4 qt_Matrix;
  float qt_Opacity;
  vec2 translate;
  float zoom;
  vec4 backgroundColor;
  vec4 fineGridColor;
  vec4 coarseGridColor;
};

float grid(vec2 fragCoord, float space, float gridWidth) {
  vec2 p = fragCoord - vec2(.5);
  vec2 size = vec2(gridWidth);

  vec2 a1 = mod(fragCoord + .5, space) - .5;
  vec2 a = abs(a1) - vec2(gridWidth / 2.);

  float g = min(a.x, a.y);
  return clamp(g, 0., 1.0);
}

void main() {
  vec2 coord = fragCoord;
  coord -= .5;
  coord -= translate;
  coord /= zoom;

  float filterCursor = min((zoom - ZOOM_CUTOFF_END) / (ZOOM_CUTOFF_START - ZOOM_CUTOFF_END), 1.);

  float smallGrid = grid(coord, SMALL_OFFSET, SMALL_WIDTH);
  float filteredSmallGrid = min(1 - smallGrid, filterCursor);
  float largeGrid =
      (1 - grid(coord, LARGE_OFFSET, LARGE_WIDTH + LARGE_WIDTH_CUT * (1.0 - filterCursor))) *
      (0.6 + filterCursor / 1.6);

  fragColor = mix(backgroundColor, fineGridColor, filteredSmallGrid);
  fragColor = mix(fragColor, coarseGridColor, largeGrid) * qt_Opacity;
}