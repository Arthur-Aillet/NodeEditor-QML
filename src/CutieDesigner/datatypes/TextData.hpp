#pragma once

#include "NodeData.hpp"

class TextData : public NodeData {
  public:
  TextData() {}
  TextData(const QString &text) {
    registerConvert([&text]() { return text; });
  }

  NodeDataType type() const override { return NodeDataType("text", "Text"); }
};
