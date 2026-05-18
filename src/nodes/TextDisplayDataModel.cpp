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
  unsigned int result = 1;

  switch (portType) {
  case PortType::In:
    result = 1;
    break;

  case PortType::Out:
    result = 0;

  default:
    break;
  }

  return result;
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

  if (!portLabel)
    return;

  portLabel->setProperty("text", _content);

  emit valueUpdated(_content);
}

void TextDisplayDataModel::onTextEdited() {
  if (!portLabel)
    return;
  _content = portLabel->property("text").toString();
  emit valueUpdated(_content);
}