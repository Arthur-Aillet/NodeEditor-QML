#pragma once

#include "NodeDelegateModel.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <qdebug.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

class TextDisplayDataModel : public NodeDelegateModel {
  Q_OBJECT

  public:
  TextDisplayDataModel();

  ~TextDisplayDataModel() = default;

  public:
  QString caption() const override { return QStringLiteral("Result"); }

  bool captionVisible() const override { return false; }

  QString name() const override { return QStringLiteral("Result"); }

  QString portCaption(PortType, PortIndex) const override { return QString(); }
  bool portCaptionVisible(PortType, PortIndex) const override { return true; }

  public:
  unsigned int nPorts(PortType portType) const override;

  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  const QUrl embeddedComponent(QQmlEngine *engine) override;
  void embeddedComponentLoaded(QObject *loaded) override;

  void inputConnectionCreated(ConnectionId const &) override {
    _connected = true;
    if (_portLabel != nullptr) {
      _portLabel->setProperty("enabled", false);
    }
  }
  void inputConnectionDeleted(ConnectionId const &) override {
    _connected = false;
    if (_portLabel != nullptr) {
      _portLabel->setProperty("enabled", true);
    }
  }

  public slots:
  void onTextEdited();

  signals:
  void valueUpdated(QString newValue);

  private:
  bool _connected = false;
  QObject *_portLabel{nullptr};

  QString _content;

  std::shared_ptr<QQmlComponent> _component{nullptr};
};
