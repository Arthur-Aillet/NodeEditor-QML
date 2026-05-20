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

  QString name() const override { return QStringLiteral("Number Source"); }

  public:
  QJsonObject save() const override;

  void load(QJsonObject const &p) override;

  public:
  unsigned int nPorts(PortType portType) const override;

  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  QString portCaption(PortType portType, PortIndex portIndex) const override { return QString(); }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

  const QUrl embeddedComponent(QQmlEngine *engine) override;

  void embeddedComponentLoaded(QObject *loaded) override;

  public:
  void setNumber(double number);

  private Q_SLOTS:

  void onTextEdited();

  private:
  QObject *_portLabel{nullptr};
  std::shared_ptr<DecimalData> _number;
  std::shared_ptr<QQmlComponent> _component{nullptr};
};