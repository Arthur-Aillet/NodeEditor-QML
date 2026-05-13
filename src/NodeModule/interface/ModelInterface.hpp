#pragma once

#include "AbstractGraphModel.hpp"
#include "Definitions.hpp"
#include "QmlUndoCommands.hpp"
#include <QObject>
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

  static ModelInterface *create(QQmlEngine *, QJSEngine *engine);
  static ModelInterface *init(AbstractGraphModel &_graphModel);

  Q_PROPERTY(AbstractGraphModel *graph READ getGraphModel);

  protected:
  AbstractGraphModel *getGraphModel() { return &graphModel; }

  inline static ModelInterface *instance = nullptr;
  ModelInterface(AbstractGraphModel &_graphModel);
  QUndoStack undoStack;

  public:
  Q_INVOKABLE QVariant nodeData(NodeId nodeId, NodeRole role);
  Q_INVOKABLE QVariant portData(NodeId nodeId, PortType portType, PortIndex index, PortRole role);
  Q_INVOKABLE bool setNodeData(NodeId nodeId, NodeRole role, QVariant value);
  Q_INVOKABLE void createConnection(ConnectionId const id);
  Q_INVOKABLE void deleteConnection(ConnectionId const id);
  Q_INVOKABLE void createNode(QString const nodeType, QPoint const &scenePos);
  Q_INVOKABLE bool deleteNode(const NodeId nodeId);

  Q_SIGNALS:
  void connectionCreated(ConnectionId const id);
  void connectionDeleted(ConnectionId const id);
  void nodeCreated(NodeId const nodeId);
  void nodeDeleted(NodeId const nodeId);
  void nodeUpdated(NodeId const nodeId);
  void nodeFlagsUpdated(NodeId const nodeId);
  void nodePositionUpdated(NodeId const nodeId);
  void modelReset();
};
