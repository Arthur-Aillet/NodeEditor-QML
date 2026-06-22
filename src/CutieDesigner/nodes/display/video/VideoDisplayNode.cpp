#include "VideoDisplayNode.hpp"
#include "GstreamerController.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qquickitem.h>
#include <qtimer.h>
#include <qtmetamacros.h>

void VideoDisplayNode::componentLoaded(QQuickItem *videoItem) {
  _controller->linkQtSink(videoItem);
  // _window->scheduleRenderJob(new SetPlaying(this, videoItem),
  //                            QQuickWindow::BeforeSynchronizingStage);
}

void VideoDisplayNode::componentDestroyed(QObject *object) {
  _controller->unlinkQtSink(qobject_cast<QQuickItem *>(object));
}

VideoDisplayNode::VideoDisplayNode(QQmlEngine *engine) : NodeDelegateModel(engine) {
  _window = CutieWindow::getCutieWindow(engine);
  _engine = engine;

  if (!_window)
    return;

  auto comp =
      std::make_unique<QQmlComponent>(_engine, "CutieDesigner.Nodes.Display", "VideoDisplay");
  QVariantMap map;
  map["node"] = QVariant::fromValue(this);
  _content = std::make_shared<SurfaceData>(std::move(comp), map);
  QObject::connect(_content.get(), SIGNAL(componentLoaded(QQuickItem *)), this,
                   SLOT(componentLoaded(QQuickItem *)));
  QObject::connect(_content.get(), SIGNAL(componentDestroyed(QObject *)), this,
                   SLOT(componentDestroyed(QObject *)));
  _controller = std::make_unique<GstreamerController>(_window);
}

unsigned int VideoDisplayNode::nPorts(PortType portType) const {
  switch (portType) {
  case PortType::In:
    return 0;
  default:
    return 1;
  }
}

const NodeDataType &VideoDisplayNode::dataType(PortType _portType, PortIndex _portIndex) const {
  return SurfaceData().type();
}

std::shared_ptr<NodeData> VideoDisplayNode::outData(PortIndex _portIndex) { return _content; }

void VideoDisplayNode::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) {}

QString VideoDisplayNode::portCaption(PortType portType, PortIndex portIndex) const {
  switch (portType) {
  case PortType::In:
    return QString();
  default:
    return QString("out");
  }
}