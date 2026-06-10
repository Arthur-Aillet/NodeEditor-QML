#include "ATypeNode.hpp"
#include "ATypeCharacterData.hpp"
#include "DecimalData.hpp"
#include "SurfaceData.hpp"
#include "TextData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <qvariant.h>

std::shared_ptr<SurfaceData> ATypeNode::createATypeSurfaceData(QQmlEngine *engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes", "AType");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  return std::make_shared<SurfaceData>(std::move(comp), map);
}

ATypeNode::ATypeNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _content = createATypeSurfaceData(engine);
}

unsigned int ATypeNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  case PortType::In:
    return 2;
  default:
    return 0;
  }
}

const NodeDataType &ATypeNode::dataType(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::Out:
    return _content->type();
  case PortType::In:
    switch (portIndex) {
    case 0:
      return TextData().type();
    default:
      return ATypeCharacterData().type();
    }
  default:
    return TextData().type();
  }
}

std::shared_ptr<NodeData> ATypeNode::outData(PortIndex) { return _content; }

void ATypeNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (portIndex == 0) {
    if (data == nullptr) {
      _text.reset();
    } else if (data->type().id == DecimalData().type().id) {
      auto numberData = std::dynamic_pointer_cast<DecimalData>(data);
      _text = numberData->numberAsText();
    } else {
      _text = std::dynamic_pointer_cast<TextData>(data);
    }

    emit textChanged();
  } else {
    if (data == nullptr) {
      _charModel = nullptr;
      emit charChanged();
    } else if (data->type().id == ATypeCharacterData().type().id) {
      _charModel = std::dynamic_pointer_cast<ATypeCharacterData>(data)->getCharacter();
      emit charChanged();
    }
  }
}
