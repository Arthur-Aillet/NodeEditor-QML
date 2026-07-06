#pragma once

#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>

class GradientData : public NodeData {
  public:
  GradientData() {}
  GradientData(const QGradient &gradientProp) {
    registerConvert([&gradientProp]() { return gradientProp; });
  }

  NodeDataType type() const override { return NodeDataType("gradient-01", "Gradient"); }
};
