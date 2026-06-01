#pragma once

#include "DecimalData.hpp"
#include "NodeData.hpp"

struct TextDataType : public NodeDataType {
  TextDataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<DataTypeId> types;
    types.push_front(id);
    types.push_front(DecimalData().type().id);
    return types;
  }
};

class TextData : public NodeData {
  public:
  TextData() {}

  TextData(QString const text) : text(text) {}

  static inline const TextDataType dataType = TextDataType("text", "Text");

  const NodeDataType &type() const override { return dataType; }

  QString text;
};
