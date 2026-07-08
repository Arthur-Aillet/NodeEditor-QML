#include "CombineColorNode.hpp"
#include "ColorData.hpp"
#include "ColorModes.hpp"
#include "CombineColorNode.hpp"
#include "DecimalData.hpp"
#include "Definitions.hpp"
#include "NodeData.hpp"
#include "vivid/color.h"
#include "vivid/types.h"
#include <qcolor.h>
#include <qlogging.h>
#include <qmetaobject.h>
#include <qobject.h>

CombineColorNode::CombineColorNode(QQmlEngine *engine)
    : NodeDelegateModel(engine), _outColorPtr(std::make_shared<ColorData>(_color)) {}

QQmlComponent CombineColorNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Process", "ColorModeControl");
}

QJsonObject CombineColorNode::save() const {
  QJsonObject json;

  json["mode"] = _mode;
  return json;
}

void CombineColorNode::load(QJsonObject const &json) {
  QJsonValue value = json["mode"];

  if (!value.isUndefined()) {
    _mode = value.toVariant().value<ColorMode>();
  }
}

void CombineColorNode::embeddedComponentLoaded(std::shared_ptr<QQuickItem> instance) {
  _embedded = instance;
  instance->setProperty("model", CutieDesigner::getColorModeNames());
  instance->setProperty("currentIndex", _mode);
  QObject::connect(instance.get(), SIGNAL(currentModeChanged()), this, SLOT(currentModeChanged()));
}

void CombineColorNode::currentModeChanged() {
  CutieDesigner::ColorMode newMode =
      static_cast<CutieDesigner::ColorMode>(_embedded.lock()->property("currentMode").toInt());
  if (_mode == newMode)
    return;

  if (_mode == CutieDesigner::CMYKA) {
    emit portsAboutToBeDeleted(PortType::In, 3, 3);
    _mode = newMode;
    emit portsDeleted(PortType::In);
  } else if (newMode == CutieDesigner::CMYKA) {
    emit portsAboutToBeInserted(PortType::In, 3, 3);
    _mode = newMode;
    emit portsInserted(PortType::In);
  } else {
    _mode = newMode;
  }

  emit portsNameChanged(PortType::In);
  emit dataUpdated(0);
}

unsigned int CombineColorNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
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

QString CombineColorNode::portCaption(PortType portType, PortIndex portIndex) const {
  if (portType == PortType::Out) {
    return "out";
  }
  return QString(CutieDesigner::getColorModeComponents(_mode)[portIndex]);
}

NodeDataType CombineColorNode::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return DecimalData().type();
  default:
    return ColorData().type();
  }
}

std::shared_ptr<NodeData> CombineColorNode::outData(PortIndex portIndex) {
  bool isFive = _mode == ColorMode::CMYKA;
  bool isOkLab = _mode == ColorMode::OKLAB;

  auto in1 = _input[0].expired() ? 0 : std::clamp(_input[0].lock()->repr<double>(), 0.0, 1.0);
  auto in2 = _input[1].expired() ? 0
                                 : std::clamp(_input[1].lock()->repr<double>(), isOkLab ? -0.5 : 0,
                                              isOkLab ? 0.5 : 1.0);
  auto in3 = _input[2].expired() ? 0
                                 : std::clamp(_input[2].lock()->repr<double>(), isOkLab ? -0.5 : 0,
                                              isOkLab ? 0.5 : 1.0);
  auto in4 = _input[3].expired() ? !isFive : std::clamp(_input[3].lock()->repr<double>(), 0.0, 1.0);
  auto in5 = _input[4].expired() ? isFive : std::clamp(_input[4].lock()->repr<double>(), 0.0, 1.0);

  vivid::Color asRgb;

  switch (_mode) {
  case RGBA:
    _color = QColor::fromRgbF(in1, in2, in3, in4);
    break;
  case HSLA:
    _color = QColor::fromHslF(in1, in2, in3, in4);
    break;
  case HSVA:
    _color = QColor::fromHsvF(in1, in2, in3, in4);
    break;
  case CMYKA:
    _color = QColor::fromCmykF(in1, in2, in3, in4, in5);
    break;
  case OKLAB:
    asRgb = vivid::Color(vivid::oklab_t{static_cast<float>(in1), static_cast<float>(in2),
                                        static_cast<float>(in3)})
                .rgb();
    _color = QColor::fromRgbF(asRgb.value().r, asRgb.value().g, asRgb.value().b, in4);
    break;
  case LCH:
    asRgb = vivid::Color(vivid::lch_t{static_cast<float>(in1) * 100, static_cast<float>(in2) * 140,
                                      static_cast<float>(in3) * 360})
                .rgb();
    _color = QColor::fromRgbF(asRgb.value().r, asRgb.value().g, asRgb.value().b, in4);
  }
  return _outColorPtr;
}

void CombineColorNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (!data) {
    _input[portIndex].reset();
  } else {
    _input[portIndex] = data;
  }
  emit dataUpdated(0);
};
