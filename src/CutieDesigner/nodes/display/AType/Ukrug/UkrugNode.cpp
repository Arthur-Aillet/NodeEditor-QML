#include "UkrugNode.hpp"
#include "ATypeCharacterData.hpp"
#include "ColorData.hpp"
#include "DecimalData.hpp"
#include <memory>
#include <qcontainerfwd.h>
#include <qdebug.h>
#include <qqmlcomponent.h>

UkrugNode::UkrugNode(QQmlEngine *engine)
    : ATypeCharacterNodeModel(engine), _modelData(std::make_shared<ATypeCharacterData>(this)),
      _component(std::move(
          std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes", "UkrugCharacter"))) {}

unsigned int UkrugNode::nPorts(PortType portType) const {
  switch (portType) {
  case NodeEditor::PortType::Out:
    return 1;
  default:
    return 1;
  }
};

const NodeDataType &UkrugNode::dataType(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case NodeEditor::PortType::Out:
    return ATypeCharacterData().type();
  default:
    switch (portIndex) {
    case 0:
      return ColorData().type();
    default:
      return DecimalData().type();
    }
  }
};

std::shared_ptr<NodeData> UkrugNode::outData(PortIndex port) { return _modelData; };

void UkrugNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {};

QQmlComponent *UkrugNode::getComponent() { return _component.get(); };

QVariantMap UkrugNode::getAdditionalProperties() {
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  map["goingToGetDestroyed"] = false;
  return map;
};