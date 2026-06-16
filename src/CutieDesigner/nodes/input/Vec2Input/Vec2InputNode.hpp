#pragma once

#include "NodeDelegateModel.hpp"
#include "Vec2Data.hpp"

class Vec2InputNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  public:
  Q_PROPERTY(double x READ x WRITE setX NOTIFY xChanged)
  Q_PROPERTY(double y READ y WRITE setY NOTIFY yChanged)

  Vec2InputNode(QQmlEngine *engine);
  ~Vec2InputNode() override = default;

  bool captionVisible() const override { return false; }
  QString name() const override { return "Vec2"; }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;
  QVariantMap componentInitialProperties() override;

  double x() { return _vec.x(); }
  double y() { return _vec.y(); }

  void setX(double x);
  void setY(double y);

  signals:
  void xChanged();
  void yChanged();

  private:
  QVector2D _vec = {0.0, 0.0};
  std::shared_ptr<Vec2Data> _vecData;
};