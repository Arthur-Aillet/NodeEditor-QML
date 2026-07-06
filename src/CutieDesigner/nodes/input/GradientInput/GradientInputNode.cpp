#include "GradientInputNode.hpp"
#include "ColorData.hpp"
#include <memory>

GradientInputNode::GradientInputNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _content(std::make_shared<ColorData>(_color)) {}

unsigned int GradientInputNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

NodeDataType GradientInputNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return ColorData().type();
}

std::shared_ptr<NodeData> GradientInputNode::outData(PortIndex _portIndex) { return _content; }
