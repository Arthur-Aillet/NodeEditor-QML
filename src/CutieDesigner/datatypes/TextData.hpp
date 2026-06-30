#pragma once

#include "NodeData.hpp"

class TextData : public NodeData {
  public:
  TextData() {}
  TextData(const QString &text) {
    registerConvert<QString>([&text]() { return text; });
  }

  static inline const NodeDataType dataType = NodeDataType("text", "Text");

  const NodeDataType &type() const override { return dataType; }
};
