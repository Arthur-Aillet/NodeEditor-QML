#pragma once

#include "CutieWindow.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

#include <gst/gstelement.h>
#include <gst/gstpipeline.h>
#include <gst/gstutils.h>

#include <QQmlComponent>
#include <QtCore/QObject>

#include <memory>
#include <qdebug.h>
#include <qjsvalue.h>
#include <qlogging.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qquickitem.h>
#include <qrunnable.h>
#include <qtmetamacros.h>

class SetPlaying : public QRunnable {
  public:
  SetPlaying(GstElement *pipeline, GstElement *sink, QQuickItem *item)
      : _pipeline(pipeline), _sink(sink), _item(item) {}

  void run() {
    g_object_set(_sink, "widget", _item, NULL);
    gst_element_set_state(_pipeline, GST_STATE_PLAYING);
  }

  private:
  GstElement *_pipeline;
  GstElement *_sink;
  QQuickItem *_item;
};

class VideoDisplayNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  VideoDisplayNode(QQmlEngine *engine);
  ~VideoDisplayNode();

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Video"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  public slots:
  void componentLoaded(QQuickItem *item) {
    _window->scheduleRenderJob(new SetPlaying(_pipeline, _sink, item),
                               QQuickWindow::AfterSynchronizingStage);
  }

  void componentDestroyed(QObject *object) {
    qDebug()
        << "finished"; // https://github.com/GStreamer/gst-plugins-good/blob/master/tests/examples/qt/qmlsink-dynamically-added/main.cpp
    gst_element_set_state(_pipeline, GST_STATE_PAUSED);
    g_object_set(_sink, "widget", NULL, NULL);
  }

  private:
  std::shared_ptr<SurfaceData> _content = nullptr;
  GstElement *_source = nullptr;
  GstElement *_glupload = nullptr;
  GstElement *_sink = nullptr;
  GstElement *_pipeline = nullptr;
  CutieWindow *_window = nullptr;
  QQmlEngine *_engine;
};
