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

class DecimalData : public TextData {
  public:
  DecimalData(const double &number = 0.0)
      : _number(number), TextData(QString::number(number, 'f', 2)) {}

  inline static const DecimalDataType dataType = DecimalDataType("decimal", "Decimal");

  const DecimalDataType &type() const override { return dataType; }

  double number() const { return _number; }

  protected:
  const double &_number;
};
