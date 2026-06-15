#pragma once

#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>

class GradientData : public NodeData {
  public:
  GradientData() {}
  GradientData(QGradient &gradientProp) {
    registerConvert<QGradient>([&gradientProp]() { return gradientProp; });
  }

  inline static const NodeDataType dataType = NodeDataType("gradient-01", "Gradient");

  const NodeDataType &type() const override { return dataType; }
};
