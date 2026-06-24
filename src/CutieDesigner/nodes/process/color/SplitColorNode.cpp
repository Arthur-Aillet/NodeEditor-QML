#include "SplitColorNode.hpp"
#include "ColorData.hpp"
#include "ColorModes.hpp"
#include "DecimalData.hpp"
#include "vivid/color.h"
#include "vivid/types.h"
#include <memory>

SplitColorNode::SplitColorNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _outValues{0, 0, 0, 0, 0},
      _outData{std::make_shared<DecimalData>(_outValues[0]),
               std::make_shared<DecimalData>(_outValues[1]),
               std::make_shared<DecimalData>(_outValues[2]),
               std::make_shared<DecimalData>(_outValues[3]),
               std::make_shared<DecimalData>(_outValues[4])} {}

QQmlComponent SplitColorNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Process", "ColorModeControl");
}

void SplitColorNode::embeddedComponentLoaded(std::shared_ptr<QQuickItem> instance) {
  _embedded = instance;
  instance->setProperty("model", CutieDesigner::getColorModeNames());
  QObject::connect(instance.get(), SIGNAL(currentModeChanged()), this, SLOT(currentModeChanged()));
}

void SplitColorNode::currentModeChanged() {
  CutieDesigner::ColorMode newMode =
      static_cast<CutieDesigner::ColorMode>(_embedded.lock()->property("currentMode").toInt());
  if (_mode == newMode)
    return;

  if (_mode == CutieDesigner::CMYKA) {
    emit portsAboutToBeDeleted(PortType::Out, 3, 3);
    _mode = newMode;
    emit portsDeleted(PortType::Out);
  } else if (newMode == CutieDesigner::CMYKA) {
    emit portsAboutToBeInserted(PortType::Out, 3, 3);
    _mode = newMode;
    emit portsInserted(PortType::Out);
  } else {
    _mode = newMode;
  }

  emit portsNameChanged(PortType::Out);
  emit dataUpdated(0);
  emit dataUpdated(1);
  emit dataUpdated(2);
  emit dataUpdated(3);
  if (_mode == ColorMode::CMYKA)
    emit dataInvalidated(4);
}

unsigned int SplitColorNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::Out:
    switch (_mode) {
    case ColorMode::CMYKA:
      return 5;
    default:
      return 4;
    }
  default:
    return 1;
  }
}

QString SplitColorNode::portCaption(PortType portType, PortIndex portIndex) const {
  if (portType == PortType::In) {
    return "in";
  }
  return QString(CutieDesigner::getColorModeComponents(_mode)[portIndex]);
}

const NodeDataType &SplitColorNode::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return ColorData().type();
  default:
    return DecimalData().type();
  }
}

std::shared_ptr<NodeData> SplitColorNode::outData(PortIndex portIndex) {
  if (_color.expired()) {
    _outValues[0] = 0;
    _outValues[1] = 0;
    _outValues[2] = 0;
    if (_mode == ColorMode::CMYKA) {
      _outValues[3] = 0;
      _outValues[4] = 1;
    } else {
      _outValues[3] = 1;
    }
    return _outData[portIndex];
  }

  const auto &color = _color.lock()->repr<QColor>();
  float in1 = 0;
  float in2 = 0;
  float in3 = 0;
  float in4 = 0;
  float in5 = 1;

  vivid::Color asVivid;

  switch (_mode) {
  case RGBA:
    color.getRgbF(&in1, &in2, &in3, &in4);
    break;
  case HSLA:
    color.getHslF(&in1, &in2, &in3, &in4);
    break;
  case HSVA:
    color.getHsvF(&in1, &in2, &in3, &in4);
    break;
  case CMYKA:
    color.getCmykF(&in1, &in2, &in3, &in4, &in5);
    break;
  case OKLAB:
    color.getRgbF(&in1, &in2, &in3, &in4);

    asVivid = vivid::Color(vivid::rgb_t{in1, in2, in3}).oklab();
    in1 = asVivid.value().x;
    in2 = asVivid.value().y;
    in3 = asVivid.value().z;
    break;
  case LCH:
    color.getRgbF(&in1, &in2, &in3, &in4);

    asVivid = vivid::Color(vivid::rgb_t{in1, in2, in3}).lch();
    in1 = asVivid.value().x / 100.0;
    in2 = asVivid.value().y / 140.0;
    in3 = asVivid.value().z / 360.0;
    break;
  }
  _outValues[0] = in1;
  _outValues[1] = in2;
  _outValues[2] = in3;
  _outValues[3] = in4;
  _outValues[4] = in5;
  return _outData[portIndex];
}

void SplitColorNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    _color.reset();
    emit dataInvalidated(0);
    emit dataInvalidated(1);
    emit dataInvalidated(2);
    emit dataInvalidated(3);
    if (_mode == ColorMode::CMYKA)
      emit dataInvalidated(4);
  } else {
    _color = data;
    emit dataUpdated(0);
    emit dataUpdated(1);
    emit dataUpdated(2);
    emit dataUpdated(3);
    if (_mode == ColorMode::CMYKA)
      emit dataInvalidated(4);
  }
};
