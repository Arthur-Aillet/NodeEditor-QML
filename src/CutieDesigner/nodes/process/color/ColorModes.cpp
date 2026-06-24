#include "ColorModes.hpp"

QStringList CutieDesigner::getColorModeNames() {
  QStringList modes;
  auto meta = QMetaEnum::fromType<ColorMode>();

  for (int i = 0; i != meta.keyCount(); i++) {
    modes.append(meta.valueToKey(i));
  }
  return modes;
}

std::string CutieDesigner::getColorModeComponents(CutieDesigner::ColorMode mode) {
  static const std::array<std::string, 5> components = {
      "rgba", "hsla", "hsva", "cmyka", "LABa",
  };

  return components[mode];
}