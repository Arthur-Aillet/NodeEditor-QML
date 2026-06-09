#include "ColorInputNode.hpp"
#include "ColorData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

ColorInputNode::ColorInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content(std::make_shared<ColorData>()) {}

unsigned int ColorInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

const NodeDataType &ColorInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return ColorData().type();
}

std::shared_ptr<NodeData> ColorInputNode::outData(PortIndex _portIndex) { return _content; }

void ColorInputNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {}

QString ColorInputNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return QString();
  default:
    return QString("out");
  }
}