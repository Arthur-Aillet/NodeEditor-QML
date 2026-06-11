#pragma once

#include "GradientData.hpp"
#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>

struct ColorDataType : public NodeDataType {
  ColorDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(GradientData().type().id);
    return types;
  }
};

class ColorData : public GradientData {
  public:
  ColorData(QColor color = "blue") : color(color), GradientData(QLinearGradient()) {
    _gradient.setColorAt(0, color);
    _gradient.setColorAt(1, color);
  }

  inline static const ColorDataType dataType = ColorDataType("col", "Color");

  const ColorDataType &type() const override { return dataType; }

  QColor color;
};
