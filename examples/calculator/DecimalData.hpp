#pragma once

#include "NodeData.hpp"

using namespace NodeEditor;

struct DecimalDataType : public NodeDataType {
  DecimalDataType();
};

class DecimalData : public NodeData {
  public:
  DecimalData() {}
  DecimalData(const double &v);

  NodeDataType type() const override { return DecimalDataType(); }
};
