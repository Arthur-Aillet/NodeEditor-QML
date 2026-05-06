#pragma once

#include "NodeGeometryInterface.hpp"
#include "QmlUndoCommands.hpp"
#include "QtNodes/internal/AbstractNodeGeometry.hpp"
#include "QtNodes/internal/Definitions.hpp"
#include <QObject>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphicsScene>
#include <memory>
#include <qdebug.h>
#include <qjsengine.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>

using QtNodes::ConnectionId;
using QtNodes::NodeId;
using QtNodes::PortIndex;

class ModelInterface : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::AbstractGraphModel &graphModel;

  static ModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static ModelInterface *init(QtNodes::AbstractGraphModel &_graphModel);

  Q_PROPERTY(NodeGeometryInterface nodeGeometry READ getNodeGeometryInterface NOTIFY
                 nodeGeometryInterfaceChanged);

  protected:
  NodeGeometryInterface getNodeGeometryInterface() {
    return NodeGeometryInterface(_nodeGeometry.get());
  }
  std::unique_ptr<QtNodes::AbstractNodeGeometry> _nodeGeometry;

  inline static ModelInterface *instance = nullptr;
  ModelInterface(QtNodes::AbstractGraphModel &_graphModel);
  QUndoStack undoStack;

  public:
  Q_INVOKABLE QVariant nodeData(NodeId nodeId, QtNodes::NodeRole role) {
    return graphModel.nodeData(nodeId, role);
  };

  Q_INVOKABLE QVariant portData(NodeId nodeId, QtNodes::PortType portType, PortIndex index,
                                QtNodes::PortRole role) {
    return graphModel.portData(nodeId, portType, index, role);
  };

  Q_INVOKABLE bool setNodeData(NodeId nodeId, QtNodes::NodeRole role, QVariant value) {
    return graphModel.setNodeData(nodeId, role, value);
  }

  Q_INVOKABLE void createConnection(NodeId inNode, PortIndex inPort, NodeId outNode,
                                    PortIndex outPort) {
    undoStack.push(new ConnectCommand(this, ConnectionId{outNode, outPort, inNode, inPort}));
  }

  Q_INVOKABLE void deleteConnection(NodeId inNode, PortIndex inPort, NodeId outNode,
                                    PortIndex outPort) {
    undoStack.push(new DisconnectCommand(this, ConnectionId{outNode, outPort, inNode, inPort}));
  }

  Q_INVOKABLE void createNode(QString const nodeType, QPoint const &scenePos) {
    undoStack.push(new CreateCommand(this, nodeType, scenePos));
  }

  void setNodeGeometry(std::unique_ptr<QtNodes::AbstractNodeGeometry> newGeom) {
    _nodeGeometry = std::move(newGeom);
    emit nodeGeometryInterfaceChanged();
  }

  Q_INVOKABLE bool deleteNode(const NodeId nodeId) {
    // TODO: restore DeleteNodeCommand
    return graphModel.deleteNode(nodeId);
  }

  public slots:
  void forwardConnectionCreated(ConnectionId const connectionId) {
    emit connectionCreated(connectionId.inNodeId, connectionId.inPortIndex, connectionId.outNodeId,
                           connectionId.outPortIndex);
  }

  void forwardConnectionDeleted(ConnectionId const connectionId) {
    emit connectionDeleted(connectionId.inNodeId, connectionId.inPortIndex, connectionId.outNodeId,
                           connectionId.outPortIndex);
  }

  Q_SIGNALS:
  void nodeGeometryInterfaceChanged();
  void connectionCreated(NodeId const inNodeId, PortIndex const inPortIndex, NodeId const outNodeId,
                         PortIndex const outPortIndex);
  void connectionDeleted(NodeId const inNodeId, PortIndex const inPortIndex, NodeId const outNodeId,
                         PortIndex const outPortIndex);
  void nodeCreated(NodeId const nodeId);
  void nodeDeleted(NodeId const nodeId);
  void nodeUpdated(NodeId const nodeId);
  void nodeFlagsUpdated(NodeId const nodeId);
  void nodePositionUpdated(NodeId const nodeId);
  void modelReset();
};
