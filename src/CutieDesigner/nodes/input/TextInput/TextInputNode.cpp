#include "TextInputNode.hpp"
#include "NodeDelegateModel.hpp"
#include "TextData.hpp"

TextInputNode::TextInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _textData(std::make_shared<TextData>(_text)) {}

QString TextInputNode::text() { return _text; }

void TextInputNode::setText(QString text) {
  if (_text == text) {
    return;
  }
  _text = text;
  emit textChanged();
  emit dataUpdated(0);
}

unsigned int TextInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

QQmlComponent TextInputNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Input", "TextInputControl");
}

QVariantMap TextInputNode::componentInitialProperties() {
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  return map;
}

const NodeDataType &TextInputNode::dataType(PortType, PortIndex id) const {
  return TextData().type();
}

std::shared_ptr<NodeData> TextInputNode::outData(PortIndex) { return _textData; }