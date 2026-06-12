#pragma once

#include "NodeData.hpp"
#include "TextData.hpp"
#include <qvariant.h>

struct DecimalDataType : public NodeDataType {
  DecimalDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(TextData().type().id);
    return types;
  }
};

class DecimalData : public NodeData {
  public:
  DecimalData() {}
  DecimalData(QProperty<double> &doubleProp) {
    _registeredBindings.push_back(doubleProp.subscribe(BindingFn([this, &doubleProp]() {
      _map.insert_or_assign(QMetaType::fromType<double>(), QVariant::fromValue(doubleProp.value()));
      _map.insert_or_assign(QMetaType::fromType<QString>(),
                            QVariant::fromValue(QString::number(doubleProp.value(), 'f', 2)));
    })));
  }

  inline static const DecimalDataType dataType = DecimalDataType("decimal", "Decimal");

  const DecimalDataType &type() const override { return dataType; }
};
