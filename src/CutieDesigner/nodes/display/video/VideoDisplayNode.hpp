#pragma once

#include "CutieWindow.hpp"
#include "GstreamerController.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

class VideoDisplayNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  VideoDisplayNode(QQmlEngine *engine);
  ~VideoDisplayNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Video"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;
  void initPipeline();

  Q_INVOKABLE void gettingDestroyed(QQuickItem *item) {
    qDebug() << "I'm getting destroyed" << item;
    _controller->unlinkQtSink(item);
  }

  public slots:
  void componentLoaded(QQuickItem *item);

  private:
  std::shared_ptr<SurfaceData> _content = nullptr;
  CutieWindow *_window = nullptr;
  std::unique_ptr<GstreamerController> _controller = nullptr;
  QQmlEngine *_engine;
};
