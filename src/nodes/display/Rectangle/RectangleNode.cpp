#include "RectangleNode.hpp"
#include "ColorData.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

RectangleNode::RectangleNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesignerModule", "RectangleDisplay");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(std::move(comp), map);
}

unsigned int RectangleNode::nPorts(PortType portType) const { return 1; }

const NodeDataType &RectangleNode::dataType(PortType portType, PortIndex _portIndex) const {
  switch (portType) {
  case PortType::In:
    return ColorData().type();
  default:
    return SurfaceData().type();
  }
}

std::shared_ptr<NodeData> RectangleNode::outData(PortIndex _portIndex) {
  if (_color.expired())
    return nullptr;
  return _content;
}

void RectangleNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  auto colorData = std::dynamic_pointer_cast<ColorData>(data);

  if (!data) {
    _color.reset();
    emit dataInvalidated(0);
  } else {
    _color = colorData;
    emit colorChanged();
    emit dataUpdated(0);
  }
}

QString RectangleNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return QString("color");
  default:
    return QString("out");
  }
}