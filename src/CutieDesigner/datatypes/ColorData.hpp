#pragma once

#include "GradientData.hpp"
#include "NodeData.hpp"
#include <QtGui/QColor>
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
  ColorData(QProperty<QColor> &colorProp) {
    _map.insert(QMetaType::fromType<QGradient>(), QVariant::fromValue(QLinearGradient()));

    defineBinding(colorProp.subscribe(BindingFn([this, &colorProp]() {
      _map.insert_or_assign(QMetaType::fromType<QColor>(), colorProp.value());
      QGradient &gradient =
          *reinterpret_cast<QGradient *>(_map[QMetaType::fromType<QGradient>()].data());
      gradient.setColorAt(0, colorProp.value());
    })));
  }

  inline static const ColorDataType dataType = ColorDataType("col", "Color");
  const ColorDataType &type() const override { return dataType; }
};
