#pragma once

#include <NodeEditor/NodeData>

using namespace NodeEditor;

class SimpleData : public NodeData {
  public:
  SimpleData() {}

  NodeDataType type() const override { return NodeDataType("simple", "SimpleType"); }
};
