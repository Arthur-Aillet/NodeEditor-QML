#include "SliderInputNode.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

SliderInputNode::SliderInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _valueData(std::make_shared<DecimalData>(_value)) {
  connect(this, &SliderInputNode::valueChanged, [&]() { emit dataUpdated(0); });
}

QJsonObject SliderInputNode::save() const { return QJsonObject({{"value", _value}}); }

void SliderInputNode::load(QJsonObject const &json) {
  QJsonValue value = json["value"];

  if (!value.isUndefined()) {
    _value = value.toDouble();
  }
}

unsigned int SliderInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

QQmlComponent SliderInputNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Input", "SliderInputControl");
}

QVariantMap SliderInputNode::componentInitialProperties() {
  return QVariantMap{{"node", QVariant::fromValue(this)}};
}

NodeDataType SliderInputNode::dataType(PortType, PortIndex id) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> SliderInputNode::outData(PortIndex) { return _valueData; }