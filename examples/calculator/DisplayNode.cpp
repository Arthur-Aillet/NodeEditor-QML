#include "DisplayNode.hpp"
#include "DecimalData.hpp"
#include "Definitions.hpp"
#include <qtmetamacros.h>

DisplayNode::DisplayNode(QQmlEngine *engine) : NodeDelegateModel(engine) {}

QString DisplayNode::name() const { return "Display"; }

unsigned int DisplayNode::nPorts(PortSide portSide) const {
  if (portSide == PortSide::Out) {
    return 0;
  } else {
    return 1;
  }
}

NodeDataType DisplayNode::dataType(PortSide _portSide, PortIndex _portIndex) const {
  return DecimalData().type();
};

bool DisplayNode::portCaptionVisible(PortSide _portSide, PortIndex _portIndex) const {
  return true;
};

QString DisplayNode::portCaption(PortSide _portSide, PortIndex _portIndex) const { return "in"; };

std::shared_ptr<NodeData> DisplayNode::outData(PortIndex _portIndex) { return nullptr; };

void DisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex _portIndex) {
  if (data == nullptr) {
    _valuePtr.reset();
  } else {
    _valuePtr = data;
  }
  emit valueChanged();
};

QQmlComponent DisplayNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "examples.calculator", "DisplayControl");
}

QVariantMap DisplayNode::componentInitialProperties() {
  return {{"node", QVariant::fromValue(this)}};
}

double DisplayNode::value() {
  if (_valuePtr.expired()) {
    return 0;
  }
  return _valuePtr.lock()->repr<double>();
};
