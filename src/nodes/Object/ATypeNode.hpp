#pragma once

#include "NodeDelegateModel.hpp"
#include "ObjectData.hpp"

#include <QQmlComponent>
#include <QtCore/QObject>
#include <qdebug.h>
#include <qjsvalue.h>
#include <qqmlcomponent.h>
#include <qqmlengine.h>
#include <qtmetamacros.h>

class ATypeNode : public NodeDelegateModel {
  Q_OBJECT

  public:
  ATypeNode(QQmlEngine *engine);

  ~ATypeNode() = default;

  public:
  QString caption() const override { return QStringLiteral("AType font display"); }

  bool captionVisible() const override { return true; }

  QString name() const override { return QStringLiteral("AType"); }

  public:
  unsigned int nPorts(PortType portType) const override;

  const NodeDataType &dataType(PortType portType, PortIndex portIndex) const override;

  std::shared_ptr<NodeData> outData(PortIndex port) override;

  void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  signals:
  void valueUpdated(QQmlComponent *component);

  private:
  bool _connected = false;

  std::shared_ptr<ObjectData> _content;
};
