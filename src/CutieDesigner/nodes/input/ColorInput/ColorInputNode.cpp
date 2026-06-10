#include "ColorInputNode.hpp"
#include "ColorData.hpp"

ColorInputNode::ColorInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content(std::make_shared<ColorData>(_color)) {}

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
