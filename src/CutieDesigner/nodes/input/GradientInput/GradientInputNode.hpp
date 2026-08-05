#pragma once

#include "GradientData.hpp"
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

  Q_PROPERTY(GradientInputList *colorList READ colorList CONSTANT)

  public:
  GradientInputNode(QQmlEngine *engine);
  ~GradientInputNode() = default;

  public:
  bool captionVisible() const override { return false; }
  QString name() const override { return "Gradient"; }

  QJsonObject save() const override;
  void load(QJsonObject const &p) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); };
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  QQmlComponent embeddedComponent(QQmlEngine *engine) override {
    return QQmlComponent(engine, "CutieDesigner.Nodes.Input", "GradientInputControl");
  }

  QVariantMap componentInitialProperties() override {
    return QVariantMap{{"node", QVariant::fromValue(this)}};
  }

  unsigned int nPorts(PortType portType) const override;
  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;
  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override {};

  GradientInputList *colorList() { return _list.get(); }

  private:
  std::shared_ptr<GradientInputList> _list;
  std::shared_ptr<GradientData> _content;
};
