#pragma once

#include "NodeData.hpp"
#include "TextData.hpp"

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
  DecimalData(QProperty<QVariant> *doubleProp) : _doubleProp(doubleProp) {
    _textRepr = QString::number(_doubleProp->value().value<double>(), 'f', 2);

    _registeredBindings.push_back(doubleProp->subscribe(BindingFn(
        [this]() { _textRepr = QString::number(_doubleProp->value().value<double>(), 'f', 2); })));
  }

  inline static const DecimalDataType dataType = DecimalDataType("decimal", "Decimal");

  const DecimalDataType &type() const override { return dataType; }

  protected:
  const QVariant &get(QMetaType type) const override {
    if (type == QMetaType::fromType<double>())
      return _doubleProp->value();
    if (type == QMetaType::fromType<QString>())
      return _textRepr;
    return err;
  }

  QProperty<QVariant> *_doubleProp;
  QVariant _textRepr;
};
