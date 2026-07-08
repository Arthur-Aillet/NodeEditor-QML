#pragma once

#include "ColorData.hpp"
#include "NodeDelegateModel.hpp"

class ColorInputNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

  public:
  ColorInputNode(QQmlEngine *engine);
  ~ColorInputNode() = default;

  public:
  bool captionVisible() const override { return false; }
  QString name() const override { return "Color"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); };
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  QVariantMap componentInitialProperties() override;

  unsigned int nPorts(PortType portType) const override;

  QJsonObject save() const override;
  void load(QJsonObject const &) override;

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override {};

  QColor color() { return _color; }

  void setColor(QColor color) {
    _color = color;
    emit colorChanged();
    emit dataUpdated(0);
  }

  signals:
  void colorChanged();

  private:
  QColor _color = "red";
  std::shared_ptr<ColorData> _content;
};
