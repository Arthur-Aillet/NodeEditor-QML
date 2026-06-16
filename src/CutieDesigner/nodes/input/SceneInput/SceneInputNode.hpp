#pragma once

#include "NodeDelegateModel.hpp"
#include "Vec2Data.hpp"

class SceneInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  SceneInputNode(QQmlEngine *engine);
  ~SceneInputNode() override = default;

  bool captionVisible() const override { return true; }
  QString name() const override { return "Scene"; }

  bool labelVisible() const override { return true; }
  QString label() const override { return "Scene dimensions"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  public slots:
  void posChanged();
  void sizeChanged();

  private:
  QVector2D _pos;
  QVector2D _size;
  QQuickItem *_frame = nullptr;
  std::shared_ptr<Vec2Data> _posData;
  std::shared_ptr<Vec2Data> _sizeData;
};