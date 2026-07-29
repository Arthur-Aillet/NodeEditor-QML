#pragma once

#include "CutieWindow.hpp"
#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

class CameraNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  CameraNode(QQmlEngine *engine);
  ~CameraNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Camera"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;
  void initPipeline();

  private:
  std::shared_ptr<SurfaceData> _content = nullptr;
  CutieWindow *_window = nullptr;
  QQmlEngine *_engine;
};
