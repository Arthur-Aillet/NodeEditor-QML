#pragma once

#include "ColorData.hpp"
#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

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
  QString name() const override { return QStringLiteral("Color Input"); }

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); };
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner", "ColorInputControl");
  }

  QVariantMap componentInitialProperties() override {
    QVariantMap map;
    map["node"] = QVariant::fromValue(this);
    return map;
  }

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override {};

  QColor color() { return _content->color; }

  void setColor(QColor color) {
    _content->color = color;
    emit colorChanged();
    emit dataUpdated(0);
  }

  signals:
  void colorChanged();

  private:
  std::shared_ptr<ColorData> _content;
};
