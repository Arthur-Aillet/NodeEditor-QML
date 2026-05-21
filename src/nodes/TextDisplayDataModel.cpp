#include "TextDisplayDataModel.hpp"
#include "DecimalData.hpp"
#include "TextData.hpp"

#include <QtWidgets/QLabel>
#include <cstdio>
#include <memory>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qtmetamacros.h>

TextDisplayDataModel::TextDisplayDataModel() {}

unsigned int TextDisplayDataModel::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 1;
  default:
    return 0;
  }
}

const NodeDataType &TextDisplayDataModel::dataType(PortType, PortIndex) const {
  return TextData().type();
}

std::shared_ptr<NodeData> TextDisplayDataModel::outData(PortIndex) { return nullptr; }

void TextDisplayDataModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr)
    return;

  if (data->type().id == DecimalData().type().id) {
    auto numberData = std::dynamic_pointer_cast<DecimalData>(data);
    _content = numberData->numberAsText();
  } else {
    auto textData = std::dynamic_pointer_cast<TextData>(data);
    _content = textData->text();
  }

  if (!_portLabel)
    return;

  _portLabel->setProperty("text", _content);

  emit valueUpdated(_content);
}

QQmlComponent TextDisplayDataModel::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesignerModule", "PortLabel");
}

void TextDisplayDataModel::embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) {
  _portLabel = loaded;
  _portLabel->setProperty("placeholderText", "Value");
  _portLabel->setProperty("enabled", !_connected);
  _portLabel->setProperty("text", _content);
  _portLabel->connect(_portLabel.get(), SIGNAL(textEdited()), this, SLOT(onTextEdited()));
}

void TextDisplayDataModel::onTextEdited() {
  if (_portLabel == nullptr)
    return;
  _content = _portLabel->property("text").toString();
  emit valueUpdated(_content);
}