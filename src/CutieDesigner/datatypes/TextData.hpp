#pragma once

#include "NodeData.hpp"

class TextData : public NodeData {
  public:
  TextData() {}
  TextData(QProperty<QString> &textProp) {
    defineBinding(textProp.subscribe(BindingFn([this, &textProp]() {
      _map.insert_or_assign(QMetaType::fromType<QString>(), textProp.value());
    })));
  }

  static inline const NodeDataType dataType = NodeDataType("text", "Text");

  const NodeDataType &type() const override { return dataType; }
};
