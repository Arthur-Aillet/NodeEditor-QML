#include "TimeInputNode.hpp"
#include "TimeController.hpp"
#include <qobjectdefs.h>

TimeInputNode::TimeInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _numberData(std::make_shared<DecimalData>(_number)) {
  if (TimeController::get()) {
    QObject::connect(TimeController::get(), SIGNAL(currentPosChanged()), this, SLOT(updateTime()));
  }
}

void TimeInputNode::updateTime() {
  _number = TimeController::get()->getCurrentPos();
  emit dataUpdated(0);
}

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