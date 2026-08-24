#include "InputNode.hpp"
#include "DecimalData.hpp"
#include "Definitions.hpp"
#include <qtmetamacros.h>

InputNode::InputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _value(0), _valuePtr(std::make_shared<DecimalData>(_value)) {}

QString InputNode::name() const { return "Input"; }

unsigned int InputNode::nPorts(PortSide portSide) const {
  if (portSide == PortSide::Out) {
    return 1;
  } else {
    return 0;
  }
}

NodeDataType InputNode::dataType(PortSide _portSide, PortIndex _portIndex) const {
  return DecimalData().type();
};

bool InputNode::portCaptionVisible(PortSide _portSide, PortIndex _portIndex) const { return true; };

QString InputNode::portCaption(PortSide _portSide, PortIndex _portIndex) const { return "out"; };

std::shared_ptr<NodeData> InputNode::outData(PortIndex _portIndex) { return _valuePtr; };

void InputNode::setInData(std::shared_ptr<NodeData> _data, PortIndex _portIndex) {};

QQmlComponent InputNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "examples.calculator", "InputControl");
}

QVariantMap InputNode::componentInitialProperties() {
  return {{"node", QVariant::fromValue(this)}};
}

double InputNode::value() { return _value; };
void InputNode::setValue(double newValue) {
  if (newValue == _value) {
    return;
  }
  _value = newValue;
  emit valueChanged();
  emit dataUpdated(0);
}
