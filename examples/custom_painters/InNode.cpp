#include "InNode.hpp"
#include "SimpleData.hpp"

using namespace NodeEditor;

InNode::InNode(QQmlEngine *engine) : NodeModel(engine) {}

QString InNode::caption() const { return "in"; };
QString InNode::name() const { return "In"; }

unsigned int InNode::nPorts(PortSide portSide) const {
  if (portSide == PortSide::Out) {
    return 1;
  } else {
    return 0;
  }
}

NodeDataType InNode::dataType(PortSide _portSide, PortIndex _portIndex) const {
  return SimpleData().type();
};

bool InNode::portCaptionVisible(PortSide portSide, PortIndex portIndex) const { return true; };
std::shared_ptr<NodeData> InNode::outData(PortIndex _portIndex) { return nullptr; };
void InNode::setInData(std::shared_ptr<NodeData> _data, PortIndex _portIndex) {};
