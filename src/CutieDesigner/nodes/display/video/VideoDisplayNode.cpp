#include "VideoDisplayNode.hpp"
#include "SurfaceData.hpp"

VideoDisplayNode::VideoDisplayNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _content = std::make_shared<SurfaceData>(
      std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "VideoDisplay"),
      QVariantMap{{"node", QVariant::fromValue(this)}});
}

unsigned int VideoDisplayNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

NodeDataType VideoDisplayNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> VideoDisplayNode::outData(PortIndex _portIndex) { return _content; }

void VideoDisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {}

QString VideoDisplayNode::portCaption(PortType portType, PortIndex portIndex) const {
  return "out";
}

QQmlComponent VideoDisplayNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "VideoDisplayControl");
};
QVariantMap VideoDisplayNode::componentInitialProperties() {
  return {{"node", QVariant::fromValue(this)}};
};

QUrl VideoDisplayNode::getSource() {
  if (_sourceUrl.has_value()) {
    return _sourceUrl.value();
  } else {
    return QUrl();
  }
}

void VideoDisplayNode::setSource(QUrl url) {
  if (_sourceUrl == url)
    return;
  _sourceUrl = url;
  emit sourceChanged();
}

QString VideoDisplayNode::getSourceFileName() {
  if (_sourceUrl.has_value()) {
    return _sourceUrl.value().fileName();
  } else {
    return "";
  }
}