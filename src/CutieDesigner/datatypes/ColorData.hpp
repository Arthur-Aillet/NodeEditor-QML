#pragma once

#include "GradientData.hpp"
#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>

struct ColorDataType : public NodeDataType {
  ColorDataType() : NodeDataType("col", "Color", {"col", "gradient-01"}) {}
};

class ColorData : public NodeData {

  public:
  ColorData() {}
  ColorData(const QColor &color) {
    registerConvert([&color]() { return color; });
    registerConvert([&color]() {
      QGradient g;
      g.setColorAt(0, color);
      return g;
    });
  }

  NodeDataType type() const override { return ColorDataType(); }
};
