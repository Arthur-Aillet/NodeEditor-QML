#pragma once

#include "ColorData.hpp"
#include "ColorModes.hpp"
#include "NodeDelegateModel.hpp"
#include <array>

using namespace CutieDesigner;

class CombineColorNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  CombineColorNode(QQmlEngine *engine);
  ~CombineColorNode() = default;

  bool captionVisible() const override { return true; }
  QString name() const override { return "Combine Color"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override;
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;

  public slots:
  void currentModeChanged();
  void embeddedComponentLoaded(std::shared_ptr<QQuickItem> instance) override;

  private:
  std::array<std::weak_ptr<NodeData>, 5> _input;
  std::weak_ptr<QQuickItem> _embedded;
  QColor _color = "black";
  ColorMode _mode = ColorMode::RGBA;
  std::shared_ptr<ColorData> _outColorPtr;
};
