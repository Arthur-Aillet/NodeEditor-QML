#include "UkrugNode.hpp"
#include "ATypeCharacterData.hpp"
#include "ColorData.hpp"
#include "DecimalData.hpp"

#include <QJsonObject>

UkrugNode::UkrugNode(QQmlEngine *engine)
    : ATypeCharacterNodeModel(engine), _modelData(std::make_shared<ATypeCharacterData>(this)),
      _component(std::move(std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display",
                                                           "UkrugCharacter"))) {}

QJsonObject UkrugNode::save() const {
  QJsonObject json = ATypeCharacterNodeModel::save();

  json["k"] = _k;
  json["circleScale"] = _circleScale;
  json["pointsScale"] = _pointsScale;
  json["smoothFactor"] = _smoothFactor;
  json["fill"] = _fill;
  json["pointsDistance"] = _pointsDistance;
  json["boxLimitX"] = _boxLimitX;
  json["boxLimitY"] = _boxLimitY;
  json["boxRadius"] = _boxRadius;
  json["substraction"] = _substraction;
  return json;
}
void UkrugNode::load(QJsonObject const &json) {
  ATypeCharacterNodeModel::load(json);

  if (!json["k"].isUndefined())
    _k = json["k"].toDouble();
  if (!json["circleScale"].isUndefined())
    _circleScale = json["circleScale"].toDouble();
  if (!json["pointsScale"].isUndefined())
    _pointsScale = json["pointsScale"].toDouble();
  if (!json["smoothFactor"].isUndefined())
    _smoothFactor = json["smoothFactor"].toDouble();
  if (!json["fill"].isUndefined())
    _fill = json["fill"].toDouble();
  if (!json["pointsDistance"].isUndefined())
    _pointsDistance = json["pointsDistance"].toDouble();
  if (!json["boxLimitX"].isUndefined())
    _boxLimitX = json["boxLimitX"].toDouble();
  if (!json["boxLimitY"].isUndefined())
    _boxLimitY = json["boxLimitY"].toDouble();
  if (!json["boxRadius"].isUndefined())
    _boxRadius = json["boxRadius"].toDouble();
  if (!json["substraction"].isUndefined())
    _substraction = json["substraction"].toBool();
}

unsigned int UkrugNode::nPorts(PortType portType) const {
  switch (portType) {
  case NodeEditor::PortType::Out:
    return 1;
  default:
    return 1;
  }
};

NodeDataType UkrugNode::dataType(PortType portType, PortIndex portIndex) const {
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

void UkrugNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  switch (portIndex) {
  case 0:
    if (!data) {
      _baseColorPtr.reset();
      emit baseColorChanged();
    } else {
      _baseColorPtr = data;
      emit baseColorChanged();
    }
  default:
    return;
  }
};

QQmlComponent *UkrugNode::getComponent() { return _component.get(); };

QVariantMap UkrugNode::getAdditionalProperties() {
  return {{"node", QVariant::fromValue(this)}, {"goingToGetDestroyed", false}};
};