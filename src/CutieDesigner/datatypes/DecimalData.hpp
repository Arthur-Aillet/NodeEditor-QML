#pragma once

#include "ColorData.hpp"
#include "GradientData.hpp"
#include "NodeData.hpp"
#include "TextData.hpp"
#include <qbrush.h>
#include <qvariant.h>

struct DecimalDataType : public NodeDataType {
  DecimalDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(TextData().type().id);
    types.push_front(ColorData().type().id);
    types.push_front(GradientData().type().id);
    return types;
  }
};

class DecimalData : public NodeData {
  public:
  DecimalData() {}
  DecimalData(QProperty<double> &doubleProp) {
    defineBinding(doubleProp.subscribe(BindingFn([this, &doubleProp]() {
      _map.insert_or_assign(QMetaType::fromType<double>(), doubleProp.value());
      _map.insert_or_assign(QMetaType::fromType<QString>(),
                            QString::number(doubleProp.value(), 'f', 2));
      QColor col = QColor::fromRgbF(doubleProp, doubleProp, doubleProp, 1);
      _map.insert_or_assign(QMetaType::fromType<QColor>(), col);
      QGradient g;
      g.setColorAt(0, col);
      _map.insert_or_assign(QMetaType::fromType<QGradient>(), QVariant::fromValue(g));
    })));
  }

  inline static const DecimalDataType dataType = DecimalDataType("decimal", "Decimal");

  const DecimalDataType &type() const override { return dataType; }
};
