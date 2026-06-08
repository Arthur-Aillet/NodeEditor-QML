#include "shaderEffects/BlendNode.hpp"
#include "BlendNode.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qdebug.h>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

BlendNode::BlendNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  auto comp = std::make_unique<QQmlComponent>(engine, "CutieDesignerModule", "Blend");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(std::move(comp), map);
}

unsigned int BlendNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  default:
    return 1;
  }
}

const NodeDataType &BlendNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> BlendNode::outData(PortIndex _portIndex) {
  qDebug() << "C";

  if (!_a.expired() && !_b.expired())
    return _content;
  return nullptr;
}

void BlendNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (data == nullptr) {
    if (portIndex == 0)
      _a.reset();
    else
      _b.reset();
    emit dataInvalidated(0);
  } else {
    if (portIndex == 0)
      _a = std::dynamic_pointer_cast<SurfaceData>(data);
    else
      _b = std::dynamic_pointer_cast<SurfaceData>(data);
    emit dataUpdated(0);
  }
}

QString BlendNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    switch (portIndex) {
    case 0:
      return QString("a");
    default:
      return QString("b");
    }
  default:
    return QString("out");
  }
}