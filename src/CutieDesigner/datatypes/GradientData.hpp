#pragma once

#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>

class GradientData : public NodeData {
  public:
  GradientData(const QGradient &gradient = QGradient()) : _gradient(gradient) {}

  inline static const NodeDataType dataType = NodeDataType("gradient-01", "Gradient");

  const NodeDataType &type() const override { return dataType; }
  const QGradient &gradient() const { return _gradient; }

  protected:
  QGradient _gradient;
};
