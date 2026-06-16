#pragma once

#include "DecimalData.hpp"
#include "NodeData.hpp"

struct Vec2DataType : public NodeDataType {
  Vec2DataType(DataTypeId id, QString name) : NodeDataType(id, name) {}

  QList<DataTypeId> compatibleTypes() const override {
    QList<NodeDataType::DataTypeId> types;
    types.push_front(id);
    types.push_front(DecimalData().type().id);
    return types;
  }
};

class Vec2Data : public NodeData {
  public:
  Vec2Data() {}
  Vec2Data(QVector2D &v) {
    registerConvert<QVector2D>([&v]() { return v; });
    registerConvert<double>([&v]() { return v.length(); });
  }

  inline static const Vec2DataType dataType = Vec2DataType("vec2-01", "Vec2");

  const Vec2DataType &type() const override { return dataType; }
};
