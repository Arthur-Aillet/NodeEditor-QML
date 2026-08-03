#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

class MaskNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceData *a READ getA NOTIFY aChanged)
  Q_PROPERTY(SurfaceData *b READ getB NOTIFY bChanged)

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

  SurfaceData *getA() { return _a.lock().get(); }
  SurfaceData *getB() { return _b.lock().get(); }

  signals:
  void aChanged();
  void bChanged();

  private:
  std::weak_ptr<SurfaceData> _a;
  std::weak_ptr<SurfaceData> _b;
  std::shared_ptr<SurfaceData> _content;
};
