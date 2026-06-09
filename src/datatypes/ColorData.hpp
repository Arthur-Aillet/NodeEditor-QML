#pragma once

#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qtmetamacros.h>

class ColorData : public NodeData {
  public:
  ColorData(QColor color = "red") : color(color) {}

  inline static const NodeDataType dataType = NodeDataType("color", "Color");

  const NodeDataType &type() const override { return dataType; }

  QColor color;
};
