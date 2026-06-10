#include "CombineRGBA.hpp"
#include "ColorData.hpp"
#include "DecimalData.hpp"
#include <algorithm>
#include <qcolor.h>

CombineRGBA::CombineRGBA(QQmlEngine *engine)
    : NodeDelegateModel(engine), _outColorPtr(std::make_shared<ColorData>(_outColor)) {}

unsigned int CombineRGBA::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 4;
  default:
    return 1;
  }
}

QString CombineRGBA::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
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
    return "out";
  }
}

const NodeDataType &CombineRGBA::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return DecimalData().type();
  default:
    return ColorData().type();
  }
}

std::shared_ptr<NodeData> CombineRGBA::outData(PortIndex portIndex) { return _outColorPtr; }

void CombineRGBA::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    switch (portIndex) {
    case 0:
      _r.reset();
      break;
    case 1:
      _g.reset();
      break;
    case 2:
      _b.reset();
      break;
    default:
      _a.reset();
      break;
    }
  } else {
    auto numberData = std::dynamic_pointer_cast<DecimalData>(data);
    switch (portIndex) {
    case 0:
      _r = numberData;
      break;
    case 1:
      _g = numberData;
      break;
    case 2:
      _b = numberData;
      break;
    default:
      _a = numberData;
      break;
    }
  }

  auto r = _r.expired() ? 0 : std::clamp(_r.lock()->number(), 0.0, 1.0);
  auto g = _g.expired() ? 0 : std::clamp(_g.lock()->number(), 0.0, 1.0);
  auto b = _b.expired() ? 0 : std::clamp(_b.lock()->number(), 0.0, 1.0);
  auto a = _a.expired() ? 1 : std::clamp(_a.lock()->number(), 0.0, 1.0);

  _outColor = QColor::fromRgbF(r, g, b, a);
  emit dataUpdated(0);
};
