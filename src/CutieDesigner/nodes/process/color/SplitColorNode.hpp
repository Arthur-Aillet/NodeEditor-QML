#pragma once

#include "ColorModes.hpp"
#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

using namespace CutieDesigner;

class SplitColorNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  SplitColorNode(QQmlEngine *engine);
  ~SplitColorNode() = default;

  public:
  bool captionVisible() const override { return true; }
  QString name() const override { return "Split Color"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;

  public slots:
  void currentModeChanged();
  void embeddedComponentLoaded(std::shared_ptr<QQuickItem> instance) override;

  private:
  std::weak_ptr<NodeData> _color;
  std::weak_ptr<QQuickItem> _embedded;
  ColorMode _mode = ColorMode::RGBA;
  std::array<double, 5> _outValues;
  std::array<std::shared_ptr<DecimalData>, 5> _outData;
};
