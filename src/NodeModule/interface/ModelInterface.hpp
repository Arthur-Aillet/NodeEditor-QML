#pragma once

#include "AbstractGraphModel.hpp"
#include "AbstractNodeGeometry.hpp"
#include "Definitions.hpp"
#include "QmlUndoCommands.hpp"
#include <QObject>
#include <memory>
#include <qdebug.h>
#include <qjsengine.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class ModelInterface : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  AbstractGraphModel &graphModel;
  std::unique_ptr<AbstractNodeGeometry> nodeGeometry;

  static ModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static ModelInterface *init(AbstractGraphModel &_graphModel);

  Q_PROPERTY(AbstractGraphModel *graph READ getGraphModel);

  Q_PROPERTY(
      AbstractNodeGeometry *nodeGeometry READ getNodeGeometry NOTIFY nodeGeometryInterfaceChanged);

  protected:
  AbstractGraphModel *getGraphModel() { return &graphModel; }
  AbstractNodeGeometry *getNodeGeometry() { return nodeGeometry.get(); }

  inline static ModelInterface *instance = nullptr;
  ModelInterface(AbstractGraphModel &_graphModel);
  QUndoStack undoStack;

  public:
  Q_INVOKABLE QVariant nodeData(NodeId nodeId, NodeRole role);
  Q_INVOKABLE QVariant portData(NodeId nodeId, PortType portType, PortIndex index, PortRole role);
  Q_INVOKABLE bool setNodeData(NodeId nodeId, NodeRole role, QVariant value);
  Q_INVOKABLE void createConnection(NodeId inNode, PortIndex inPort, NodeId outNode,
                                    PortIndex outPort);
  Q_INVOKABLE void deleteConnection(NodeId inNode, PortIndex inPort, NodeId outNode,
                                    PortIndex outPort);
  Q_INVOKABLE void createNode(QString const nodeType, QPoint const &scenePos);
  Q_INVOKABLE bool deleteNode(const NodeId nodeId);

  void setNodeGeometry(std::unique_ptr<AbstractNodeGeometry> newGeom) {
    nodeGeometry = std::move(newGeom);
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
