#pragma once

#include "GradientData.hpp"
#include "NodeData.hpp"
#include <QtGui/QColor>
#include <qbrush.h>
#include <qcolor.h>

struct ColorDataType : public NodeDataType {
  ColorDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(GradientData().type().id);
    return types;
  }
};

class ColorData : public NodeData {

  public:
  ColorData() {}
  ColorData(const QColor &color) {
    registerConvert<QColor>([&color]() { return color; });
    registerConvert<QGradient>([&color]() {
      QGradient g;
      g.setColorAt(0, color);
      return g;
    });
  }

  inline static const ColorDataType dataType = ColorDataType("col", "Color");
  const ColorDataType &type() const override { return dataType; }
};
