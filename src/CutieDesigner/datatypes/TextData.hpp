#pragma once

#include "NodeData.hpp"

class TextData : public NodeData {
  public:
  TextData(const QString &text = "") : _text(text) {}

  static inline const NodeDataType dataType = NodeDataType("text", "Text");

  const NodeDataType &type() const override { return dataType; }
  const QString &text() const { return _text; }

  protected:
  const QString &_text;
};
