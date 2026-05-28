#include "ATypeNode.hpp"
#include "NodeDelegateModel.hpp"
#include "ObjectData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

ATypeNode::ATypeNode(QQmlEngine *engine)
    : NodeDelegateModel(engine),
      _content(std::make_shared<ObjectData>(
          std::make_unique<QQmlComponent>(engine, "CutieDesignerModule", "AType"))) {}

unsigned int ATypeNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

const NodeDataType &ATypeNode::dataType(PortType, PortIndex) const { return _content->type(); }

std::shared_ptr<NodeData> ATypeNode::outData(PortIndex) { return _content; }

void ATypeNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) { return; }
