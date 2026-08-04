#include "MaskNode.hpp"
#include "ColorData.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"
#include <qcolor.h>

MaskNode::MaskNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "Mask");
  _content = std::make_shared<SurfaceData>(std::move(comp),
                                           QVariantMap{{"node", QVariant::fromValue(this)}});
}

QJsonObject MaskNode::save() const {
  return QJsonObject({{"mode", _mode}, {"inverted", _inverted}});
}

void MaskNode::load(QJsonObject const &json) {
  QJsonValue valueMode = json["mode"];

  if (!valueMode.isUndefined()) {
    _mode = valueMode.toInt();
  }

  QJsonValue valueInverted = json["inverted"];

  if (!valueInverted.isUndefined()) {
    _inverted = valueInverted.toBool();
  }
}

unsigned int MaskNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 7;
  default:
    return 1;
  }
}

NodeDataType MaskNode::dataType(PortType portType, PortIndex portIndex) const {
  if (portType == PortType::In) {
    if (portIndex == 2)
      return ColorData().type();
    if (portIndex > 2)
      return DecimalData().type();
    return SurfaceData().type();
  } else {
    return SurfaceData().type();
  }
}

std::shared_ptr<NodeData> MaskNode::outData(PortIndex _portIndex) {
  if (_image.expired() && _mask.expired())
    return nullptr;
  return _content;
}

void MaskNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    switch (portIndex) {
    case 0:
      _image.reset();
      emit imageChanged();
      emit dataInvalidated(0);
      return;
    case 1:
      _mask.reset();
      emit maskChanged();
      emit dataInvalidated(0);
      return;
    case 2:
      _color = QColor("red");
      emit colorChanged();
      return;
    case 3:
      _spreadMin = 0.0;
      emit spreadMinChanged();
      return;
    case 4:
      _spreadMax = 1.0;
      emit spreadMaxChanged();
      return;
    case 5:
      _tresholdMin = 0.0;
      emit tresholdMinChanged();
      return;
    case 6:
      _tresholdMax = 0.5;
      emit tresholdMaxChanged();
      return;
    }
  } else {
    switch (portIndex) {
    case 0:
      _image = std::dynamic_pointer_cast<SurfaceData>(data);
      emit imageChanged();
      emit dataUpdated(0);
      return;
    case 1:
      _mask = std::dynamic_pointer_cast<SurfaceData>(data);
      emit maskChanged();
      emit dataUpdated(0);
      return;
    case 2:
      _color = data->repr<QColor>();
      emit colorChanged();
      return;
    case 3:
      _spreadMin = data->repr<double>();
      emit spreadMinChanged();
      return;
    case 4:
      _spreadMax = data->repr<double>();
      emit spreadMaxChanged();
      return;
    case 5:
      _tresholdMin = data->repr<double>();
      emit tresholdMinChanged();
      return;
    case 6:
      _tresholdMax = data->repr<double>();
      emit tresholdMaxChanged();
      return;
    }
  }
}

QString MaskNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return "image";
    case 1:
      return "mask";
    case 2:
      return "color";
    case 3:
      return "spread min";
    case 4:
      return "spread max";
    case 5:
      return "treshold min";
    case 6:
      return "treshold max";
    }
  default:
    return "out";
  }
}