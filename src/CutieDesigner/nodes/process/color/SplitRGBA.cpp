#include "SplitRGBA.hpp"
#include "ColorData.hpp"
#include "DecimalData.hpp"

SplitRGBA::SplitRGBA(QQmlEngine *engine)
    : NodeDelegateModel(engine), _r(std::make_shared<DecimalData>(0)),
      _g(std::make_shared<DecimalData>(0)), _b(std::make_shared<DecimalData>(0)),
      _a(std::make_shared<DecimalData>(1)) {}

unsigned int SplitRGBA::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 1;
  default:
    return 4;
  }
}

QString SplitRGBA::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::Out:
    switch (portIndex) {
    case 0:
      return "r";
    case 1:
      return "g";
    case 2:
      return "b";
    default:
      return "a";
    }
  default:
    return "in";
  }
}

const NodeDataType &SplitRGBA::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return ColorData().type();
  default:
    return DecimalData().type();
  }
}

std::shared_ptr<NodeData> SplitRGBA::outData(PortIndex portIndex) {
  switch (portIndex) {
  case 0:
    return _r;
  case 1:
    return _g;
  case 2:
    return _b;
  default:
    return _a;
  }
}

void SplitRGBA::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    _inColor.reset();
    _r->number = 0;
    _g->number = 0;
    _b->number = 0;
    _a->number = 1;
    emit dataInvalidated(0);
    emit dataInvalidated(1);
    emit dataInvalidated(2);
    emit dataInvalidated(3);
  } else {
    auto colorData = std::dynamic_pointer_cast<ColorData>(data);
    _inColor = colorData;
    _r->number = colorData->color.redF();
    _g->number = colorData->color.greenF();
    _b->number = colorData->color.blueF();
    _a->number = colorData->color.alphaF();
    emit dataUpdated(0);
    emit dataUpdated(1);
    emit dataUpdated(2);
    emit dataUpdated(3);
  }
};
