#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

class MaskNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceData *image READ image NOTIFY imageChanged)
  Q_PROPERTY(SurfaceData *mask READ mask NOTIFY maskChanged)

  MaskNode(QQmlEngine *engine);
  ~MaskNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Mask"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  SurfaceData *image() { return _image.lock().get(); }
  SurfaceData *mask() { return _mask.lock().get(); }

  signals:
  void imageChanged();
  void maskChanged();

  private:
  std::weak_ptr<SurfaceData> _image;
  std::weak_ptr<SurfaceData> _mask;
  std::shared_ptr<SurfaceData> _content;
};
