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
  Q_INVOKABLE QVariant nodeData(NodeId nodeId, QtNodes::NodeRole role);
  Q_INVOKABLE QVariant portData(NodeId nodeId, QtNodes::PortType portType, PortIndex index,
                                QtNodes::PortRole role);
  Q_INVOKABLE bool setNodeData(NodeId nodeId, QtNodes::NodeRole role, QVariant value);
  Q_INVOKABLE void createConnection(NodeId inNode, PortIndex inPort, NodeId outNode,
                                    PortIndex outPort);
  Q_INVOKABLE void deleteConnection(NodeId inNode, PortIndex inPort, NodeId outNode,
                                    PortIndex outPort);
  Q_INVOKABLE void createNode(QString const nodeType, QPoint const &scenePos);
  Q_INVOKABLE bool deleteNode(const NodeId nodeId);

  void setNodeGeometry(std::unique_ptr<QtNodes::AbstractNodeGeometry> newGeom) {
    _nodeGeometry = std::move(newGeom);
    emit nodeGeometryInterfaceChanged();
  }

  public slots:
  void forwardConnectionCreated(ConnectionId const connectionId);
  void forwardConnectionDeleted(ConnectionId const connectionId);

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
