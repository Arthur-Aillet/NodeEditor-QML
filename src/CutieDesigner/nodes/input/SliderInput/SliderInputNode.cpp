#include "SliderInputNode.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

SliderInputNode::SliderInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _valueData(std::make_shared<DecimalData>(_value)) {}

QJsonObject SliderInputNode::save() const {
  return QJsonObject({{"value", _value}, {"min", _min}, {"max", _max}});
}

void SliderInputNode::load(QJsonObject const &json) {
  QJsonValue value = json["value"];

  if (!value.isUndefined()) {
    _value = value.toDouble();
  }

  QJsonValue min = json["min"];

  if (!min.isUndefined()) {
    _min = min.toDouble();
  }

  QJsonValue max = json["max"];

  if (!max.isUndefined()) {
    _max = max.toDouble();
  }
}

void SliderInputNode::setValue(double value) {
  if (_value == value)
    return;
  _value = value;
  emit valueChanged();
  emit dataUpdated(0);
}

void SliderInputNode::setMin(double min) {
  if (_min == min)
    return;
  _min = min;
  emit minChanged();
}

void SliderInputNode::setMax(double max) {
  if (_max == max)
    return;
  _max = max;
  emit maxChanged();
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