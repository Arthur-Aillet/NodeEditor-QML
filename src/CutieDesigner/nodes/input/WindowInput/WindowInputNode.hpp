#pragma once

#include "CutieWindow.hpp"
#include "NodeDelegateModel.hpp"
#include "Vec2Data.hpp"

class WindowInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  WindowInputNode(QQmlEngine *engine);
  ~WindowInputNode() override = default;

  bool captionVisible() const override { return true; }
  QString name() const override { return "Window"; }

  bool labelVisible() const override { return true; }
  QString label() const override { return "Cutie window dimensions"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  public slots:
  void resizeEvent(QResizeEvent *event);

  private:
  QVector2D _size;
  CutieWindow *_window = nullptr;
  std::shared_ptr<Vec2Data> _sizeData;
};