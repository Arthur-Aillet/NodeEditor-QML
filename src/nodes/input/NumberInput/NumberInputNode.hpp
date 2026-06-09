#pragma once

#include "DecimalData.hpp"
#include "NodeDelegateModel.hpp"

#include <QQuickItem>
#include <QtCore/QObject>
#include <qforeach.h>
#include <qobject.h>
#include <qqmlengine.h>

class NumberInputNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  NumberInputNode(QQmlEngine *engine);
  ~NumberInputNode() override = default;

  public:
  bool captionVisible() const override { return false; }
  QString name() const override { return "Number Input"; }

  QJsonObject save() const override;
  void load(QJsonObject const &p) override;

  unsigned int nPorts(PortType portType) const override;
  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;
  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  QQmlComponent embeddedComponent(QQmlEngine *engine) override;

  void embeddedComponentLoaded(std::shared_ptr<QQuickItem> loaded) override;

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited();

  private:
  std::shared_ptr<QQuickItem> _portLabel{nullptr};
  std::shared_ptr<DecimalData> _number;
  std::shared_ptr<QQmlComponent> _component{nullptr};
};