#include "CombineVec2Node.hpp"
#include "DecimalData.hpp"
#include "NodeData.hpp"
#include "Vec2Data.hpp"

CombineVec2Node::CombineVec2Node(QQmlEngine *engine)
    : NodeDelegateModel(engine), _vecData(std::make_shared<Vec2Data>(_vec)) {}

unsigned int CombineVec2Node::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  default:
    return 1;
  }
}

QString CombineVec2Node::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return "x";
    default:
      return "y";
    }
  default:
    return "out";
  }
}

NodeDataType CombineVec2Node::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return DecimalData().type();
  default:
    return Vec2Data().type();
  }
}

std::shared_ptr<NodeData> CombineVec2Node::outData(PortIndex portIndex) { return _vecData; }

void CombineVec2Node::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  double value;

  if (!data) {
    value = 0;
  } else {
    value = data->repr<double>();
  }

  if (portIndex == 0) {
    if (_vec.x() == value)
      return;

    _vec.setX(value);
  } else {
    if (_vec.y() == value)
      return;

    _vec.setY(value);
  }
  emit dataUpdated(0);
}