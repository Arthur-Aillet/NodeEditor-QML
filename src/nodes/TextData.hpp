#pragma once

#include "DecimalData.hpp"
#include "NodeData.hpp"
#include <qdebug.h>
#include <qlogging.h>

class TextDataType : public NodeDataType {
  public:
  TextDataType(QString id, QString name) : NodeDataType(id, name) {}

  QList<QString> compatibleTypes() override {
    QList<QString> types;
    types.push_front(id);
    types.push_front(DecimalData().type().id);
    return types;
  }
};

class TextData : public NodeData {
  public:
  TextData() : _text() {}

  TextData(QString const text) : _text(text) {}

  static inline const TextDataType dataType = TextDataType("text", "Text");

  const NodeDataType &type() const override { return dataType; }

  QString text() const { return _text; }

  private:
  QString _text;
};
