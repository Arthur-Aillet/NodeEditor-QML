#pragma once

#include "NodeData.hpp"

struct Vec2DataType : public NodeDataType {
  Vec2DataType();
};

class Vec2Data : public NodeData {
  public:
  Vec2Data() {}
  Vec2Data(const QVector2D &v);

  NodeDataType type() const override { return Vec2DataType(); }
};
