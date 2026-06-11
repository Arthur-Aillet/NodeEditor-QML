#include "SplitRGBA.hpp"
#include "ColorData.hpp"
#include "DecimalData.hpp"
#include <memory>

SplitRGBA::SplitRGBA(QQmlEngine *engine)
    : NodeDelegateModel(engine), _rPtr(std::make_shared<DecimalData>(_r)),
      _gPtr(std::make_shared<DecimalData>(_g)), _bPtr(std::make_shared<DecimalData>(_b)),
      _aPtr(std::make_shared<DecimalData>(_a)) {}

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
    return _rPtr;
  case 1:
    return _gPtr;
  case 2:
    return _bPtr;
  default:
    return _aPtr;
  }
}

void SplitRGBA::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    _inColor.reset();
    _r = 0.0;
    _g = 0.0;
    _b = 0.0;
    _a = 1.0;
    emit dataInvalidated(0);
    emit dataInvalidated(1);
    emit dataInvalidated(2);
    emit dataInvalidated(3);
  } else {
    auto colorData = std::dynamic_pointer_cast<ColorData>(data);
    _inColor = colorData;
    _r = colorData->color().redF();
    _g = colorData->color().greenF();
    _b = colorData->color().blueF();
    _a = colorData->color().alphaF();
    emit dataUpdated(0);
    emit dataUpdated(1);
    emit dataUpdated(2);
    emit dataUpdated(3);
  }
};
