#include "ATypeNode.hpp"
#include "ObjectData.hpp"
#include "TextData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <qvariant.h>

std::shared_ptr<ObjectData> ATypeNode::createATypeObjectData(QQmlEngine *engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesignerModule", "AType");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  return std::make_shared<ObjectData>(std::move(comp), map);
}

ATypeNode::ATypeNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _content = createATypeObjectData(engine);
}

unsigned int ATypeNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 1;
  }
}

const NodeDataType &ATypeNode::dataType(PortType portType, PortIndex) const {
  switch (portType) {
  case PortType::Out:
    return _content->type();
  default:
    return TextData().type();
  }
}

std::shared_ptr<NodeData> ATypeNode::outData(PortIndex) { return _content; }

void ATypeNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    _text = nullptr;
    emit textChanged();
    return;
  }

  if (data->type().id == DecimalData().type().id) {
    auto numberData = std::dynamic_pointer_cast<DecimalData>(data);
    _text = std::make_shared<TextData>(numberData->numberAsText());
  } else {
    auto textData = std::dynamic_pointer_cast<TextData>(data);
    _text = textData;
  }

  emit textChanged();
}
