#include "TimeInputNode.hpp"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QLineEdit>
#include <qqmlcomponent.h>

TimeInputNode::TimeInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _numberData(std::make_shared<DecimalData>(_number)) {}

unsigned int TimeInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    return 1;
  default:
    return 0;
  }
}

const NodeDataType &TimeInputNode::dataType(PortType, PortIndex id) const {
  return DecimalData().type();
}

std::shared_ptr<NodeData> TimeInputNode::outData(PortIndex) { return _numberData; }