#pragma once

#include "ColorData.hpp"
#include "GradientInputList.hpp"
#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <memory>
#include <qcolor.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class GradientInputNode : public NodeDelegateModel {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("NodeDelegateModel")

  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(GradientInputList *colorList READ colorList CONSTANT)

  public:
  GradientInputNode(QQmlEngine *engine);
  ~GradientInputNode() = default;

  public:
  bool captionVisible() const override { return false; }
  QString name() const override { return "Gradient"; }

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); };
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner.Nodes", "GradientInputControl");
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

  GradientInputList *colorList() { return nullptr; }

  QColor color() { return _color.value(); }

  void setColor(QColor color) {
    _color = color;
    emit colorChanged();
    emit dataUpdated(0);
  }

  signals:
  void colorChanged();

  private:
  QProperty<QColor> _color = QProperty<QColor>(QColor("red"));
  std::shared_ptr<ColorData> _content;
};
