#pragma once

#include <NodeEditor/NodeData>

using namespace NodeEditor;

class DecimalData : public NodeData {
  public:
  DecimalData() {}
  DecimalData(const double &v) {
    registerConvert([&v]() { return v; });
  }

  NodeDataType type() const override { return NodeDataType("decimal", "Decimal"); }
};
