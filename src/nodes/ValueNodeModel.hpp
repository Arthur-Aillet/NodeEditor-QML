#pragma once

#include "NodeDelegateModel.hpp"
#include "nodes/DecimalData.hpp"

#include <QQuickItem>
#include <QtCore/QObject>
#include <qforeach.h>
#include <qobject.h>
#include <qqmlengine.h>

class NumberData;

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ValueNodeModel : public NodeDelegateModel {
  Q_OBJECT

  public:
  ValueNodeModel();

  ~ValueNodeModel() override = default;

  public:
  QString caption() const override { return QStringLiteral("Number Source"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("NumberSource"); }

  public:
  QJsonObject save() const override;

  void load(QJsonObject const &p) override;

  public:
  unsigned int nPorts(PortType portType) const override;

  NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  std::shared_ptr<QQmlComponent> embeddedComponent(QQmlEngine *engine) override {
    if (_component == nullptr) {
      _component = std::make_shared<QQmlComponent>(engine, "CutieDesignerModule", "PortLabel");
    }

    return _component;
  }
  QObject *portLabel{nullptr};

  void embeddedComponentLoaded(QObject *loaded) override {
    portLabel = loaded;
    portLabel->setProperty("placeholderText", "Value");
    portLabel->connect(portLabel, SIGNAL(textEdited()), this, SLOT(onTextEdited()));
  }

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited();

  private:
  std::shared_ptr<DecimalData> _number;

  std::shared_ptr<QQmlComponent> _component{nullptr};
};