#include "VideoDisplayNode.hpp"
#include "SurfaceData.hpp"

#include <QtWidgets/QLabel>
#include <memory>
#include <qdebug.h>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qtimer.h>
#include <qtmetamacros.h>

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

  _pipeline = gst_pipeline_new(NULL);
  _source = gst_element_factory_make("videotestsrc", NULL);
  _glupload = gst_element_factory_make("glupload", NULL);
  _sink = gst_element_factory_make("qml6glsink", NULL);

  g_assert(_source && _glupload && _sink);

  gst_bin_add_many(GST_BIN(_pipeline), _source, _glupload, _sink, NULL);
  gst_element_link_many(_source, _glupload, _sink, NULL);
}

VideoDisplayNode::~VideoDisplayNode() {
  if (_pipeline != nullptr) {
    gst_element_set_state(_pipeline, GST_STATE_NULL);
    gst_object_unref(_pipeline);
  }
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