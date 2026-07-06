#include "ImageDisplayNode.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qquickitem.h>
#include <qtimer.h>
#include <qtmetamacros.h>

ImageDisplayNode::ImageDisplayNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(
      std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "ImageDisplay"), map);
}

unsigned int ImageDisplayNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

NodeDataType ImageDisplayNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> ImageDisplayNode::outData(PortIndex _portIndex) { return _content; }

void ImageDisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {}

QString ImageDisplayNode::portCaption(PortType portType, PortIndex portIndex) const {
  return "out";
}

QQmlComponent ImageDisplayNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "ImageDisplayControl");
};
QVariantMap ImageDisplayNode::componentInitialProperties() {
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  return map;
};

QUrl ImageDisplayNode::getSource() {
  if (_sourceUrl.has_value()) {
    return _sourceUrl.value();
  } else {
    return QUrl();
  }
}

void ImageDisplayNode::setSource(QUrl url) {
  if (_sourceUrl == url)
    return;
  _sourceUrl = url;
  emit sourceChanged();
}

QString ImageDisplayNode::getSourceFileName() {
  if (_sourceUrl.has_value()) {
    return _sourceUrl.value().fileName();
  } else {
    return "";
  }
}