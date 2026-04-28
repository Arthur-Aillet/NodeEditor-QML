#pragma once

#include "QtNodes/internal/Definitions.hpp"
#include "QmlUndoCommands.hpp"
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
#include <qqmlengine.h>

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class ModelInterface : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::AbstractGraphModel &graphModel;

  static ModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static ModelInterface *init(QtNodes::AbstractGraphModel &_graphModel);
  
  protected:
  inline static ModelInterface *instance = nullptr;
  ModelInterface(QtNodes::AbstractGraphModel &_graphModel);
  QUndoStack undoStack;

  public:
  Q_INVOKABLE QVariant nodeData(NodeId nodeId, QtNodes::NodeRole role) {
    return graphModel.nodeData(nodeId, role);
  };

  Q_INVOKABLE QVariant portData(NodeId nodeId, QtNodes::PortType portType, QtNodes::PortIndex index, QtNodes::PortRole role) {
    return graphModel.portData(nodeId, portType, index, role);
  };
  
  Q_INVOKABLE bool setNodeData(NodeId nodeId, QtNodes::NodeRole role, QVariant value) {
    return graphModel.setNodeData(nodeId, role, value);
  }

  Q_INVOKABLE void createNode(QString const nodeType, QPoint const &scenePos) {
    undoStack.push(new CreateCommand(this, nodeType, scenePos));
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
