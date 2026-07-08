#include "DecimalData.hpp"
#include "SplitVec2Node.hpp"
#include "Vec2Data.hpp"
#include <memory>
#include <qcolor.h>
#include <qvectornd.h>

SplitVec2Node::SplitVec2Node(QQmlEngine *engine)
    : NodeDelegateModel(engine), _xData(std::make_shared<DecimalData>(_x)),
      _yData(std::make_shared<DecimalData>(_y)) {}

unsigned int SplitVec2Node::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 1;
  default:
    return 2;
  }
}

QString SplitVec2Node::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::Out:
    switch (portIndex) {
    case 0:
      return "x";
    default:
      return "y";
    }
  default:
    return "in";
  }
}

NodeDataType SplitVec2Node::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return Vec2Data().type();
  default:
    return DecimalData().type();
  }
}

std::shared_ptr<NodeData> SplitVec2Node::outData(PortIndex portIndex) {
  switch (portIndex) {
  case 0:
    return _xData;
  default:
    return _yData;
  }
}

void SplitVec2Node::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    _x = 0.0;
    _y = 0.0;
    emit dataInvalidated(0);
    emit dataInvalidated(1);
  } else {
    const auto &vec = data->repr<QVector2D>();
    _x = vec.x();
    _y = vec.y();
    emit dataUpdated(0);
    emit dataUpdated(1);
  }
};
