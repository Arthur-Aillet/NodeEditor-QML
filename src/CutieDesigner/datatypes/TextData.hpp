#pragma once

#include "NodeData.hpp"

class TextData : public NodeData {
  public:
  TextData() {}
  TextData(QProperty<QVariant> *textProp) : _textProp(textProp) {}

  static inline const NodeDataType dataType = NodeDataType("text", "Text");

  const NodeDataType &type() const override { return dataType; }

  protected:
  const QVariant &get(QMetaType type) const override {
    if (type == QMetaType::fromType<QString>())
      return _textProp->value();
    return err;
  }

  protected:
  QProperty<QVariant> *_textProp;
};
