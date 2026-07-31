#include "ImageDisplayNode.hpp"
#include "Definitions.hpp"
#include "SurfaceData.hpp"
#include "Vec2Data.hpp"
#include <qdebug.h>
#include <qvectornd.h>

ImageDisplayNode::ImageDisplayNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _content = std::make_shared<SurfaceData>(
      std::make_unique<QQmlComponent>(engine, "CutieDesigner.Nodes.Display", "ImageDisplay"),
      QVariantMap{{"node", QVariant::fromValue(this)}});
}

unsigned int ImageDisplayNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 2;
  default:
    return 1;
  }
}

NodeDataType ImageDisplayNode::dataType(PortType portType, PortIndex portIndex) const {
  if (portType == PortType::In) {
    return Vec2Data().type();
  } else {
    return SurfaceData().type();
  }
}

std::shared_ptr<NodeData> ImageDisplayNode::outData(PortIndex _portIndex) { return _content; }

void ImageDisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {
  if (portIndex == 0) {
    if (data == nullptr) {
      _pos = QVector2D(0, 0);
    } else {
      _pos = data->repr<QVector2D>();
    }
    emit posChanged();
  } else {
    if (data == nullptr) {
      _size = QVector2D(0, 0);
      _sizeSet = false;
    } else {
      _size = data->repr<QVector2D>();
      _sizeSet = true;
    }
    emit sizeChanged();
  }
}

QString ImageDisplayNode::portCaption(PortType portType, PortIndex portIndex) const {
  if (portType == PortType::In) {
    if (portIndex == 0) {
      return "pos";
    } else {
      return "scale";
    }
  } else {
    return "out";
  }
}

QQmlComponent ImageDisplayNode::embeddedComponent(QQmlEngine *engine) {
  return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "ImageDisplayControl");
};
QVariantMap ImageDisplayNode::componentInitialProperties() {
  return {{"node", QVariant::fromValue(this)}};
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

bool ImageDisplayNode::tile() { return _tile; }
void ImageDisplayNode::setTile(bool tile) {
  if (_tile == tile)
    return;
  _tile = tile;
  emit tileChanged();
}
