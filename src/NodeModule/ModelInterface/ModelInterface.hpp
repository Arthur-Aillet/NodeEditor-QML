#pragma once

#include "QtNodes/internal/Definitions.hpp"
#include <QObject>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphicsScene>
#include <qdebug.h>
#include <qjsengine.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class ModelInterface : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::AbstractGraphModel &graphModel;

  inline static ModelInterface *instance = nullptr;

  static ModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static ModelInterface *init(QtNodes::AbstractGraphModel &_graphModel);

  protected:
  ModelInterface(QtNodes::AbstractGraphModel &_graphModel);

  public:
  Q_INVOKABLE QVariant nodeData(NodeId nodeId, QtNodes::NodeRole role) {
    return graphModel.nodeData(nodeId, role);
  };

  Q_INVOKABLE NodeId addNode(QString const nodeType = QString()) {
    return graphModel.addNode(nodeType);
  }

  Q_INVOKABLE bool deleteNode(const NodeId nodeId) { return graphModel.deleteNode(nodeId); }

  Q_SIGNALS:
  void connectionCreated(ConnectionId const connectionId);
  void connectionDeleted(ConnectionId const connectionId);
  void nodeCreated(NodeId const nodeId);
  void nodeDeleted(NodeId const nodeId);
  void nodeUpdated(NodeId const nodeId);
  void nodeFlagsUpdated(NodeId const nodeId);
  void nodePositionUpdated(NodeId const nodeId);
  void modelReset();
};
