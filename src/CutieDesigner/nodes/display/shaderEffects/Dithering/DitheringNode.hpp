#pragma once

#include "NodeDelegateModel.hpp"
#include "SurfaceData.hpp"

class DitheringNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(SurfaceData *surface READ surface NOTIFY surfaceChanged)
  Q_PROPERTY(int mode READ getMode WRITE setMode NOTIFY modeChanged)
  Q_PROPERTY(int resolution READ resolution NOTIFY resolutionChanged)

  DitheringNode(QQmlEngine *engine);
  ~DitheringNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Dithering"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner.Nodes.Display", "DitheringControl");
  }

  QVariantMap componentInitialProperties() override {
    return QVariantMap{{"node", QVariant::fromValue(this)}};
  }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  SurfaceData *surface() { return _surface.lock().get(); }
  int resolution() { return _resolution; }
  int getMode() { return _mode; }
  void setMode(int mode) {
    if (mode == _mode)
      return;
    _mode = mode;
    emit modeChanged();
  }

  signals:
  void surfaceChanged();
  void modeChanged();
  void resolutionChanged();

  private:
  int _mode = 0;
  int _resolution = 1;
  std::weak_ptr<SurfaceData> _surface;
  std::shared_ptr<SurfaceData> _content;
};
