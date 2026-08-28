#include "OutNode.hpp"
#include "SimpleData.hpp"

OutNode::OutNode(QQmlEngine *engine) : NodeModel(engine) {}

QString OutNode::caption() const { return "out"; };
QString OutNode::name() const { return "Out"; }

unsigned int OutNode::nPorts(PortSide portSide) const {
  if (portSide == PortSide::Out) {
    return 0;
  } else {
    return 1;
  }
}

NodeDataType OutNode::dataType(PortSide _portSide, PortIndex _portIndex) const {
  return SimpleData().type();
};

bool OutNode::portCaptionVisible(PortSide portSide, PortIndex portIndex) const { return true; };
std::shared_ptr<NodeData> OutNode::outData(PortIndex _portIndex) { return nullptr; };
void OutNode::setInData(std::shared_ptr<NodeData> data, PortIndex _portIndex) {}
