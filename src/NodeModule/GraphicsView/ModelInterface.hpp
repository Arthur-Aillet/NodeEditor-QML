#pragma once

#include "NodeModel.hpp"
#include "QtNodes/internal/BasicGraphicsScene.hpp"
#include "QtNodes/internal/DataFlowGraphModel.hpp"
#include "QtNodes/internal/QmlWrapper.hpp"
#include "ValueNodeModel.hpp"
#include <QDebug>
#include <QObject>
#include <QtNodes/AbstractGraphModel>
#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/DataFlowGraphicsScene>
#include <qdebug.h>
#include <qjsengine.h>
#include <qlogging.h>
#include <qloggingcategory.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

using QtNodes::ConnectionId;
using QtNodes::NodeId;

class ModelInterface : public QObject {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::AbstractGraphModel &graphModel;

  inline static ModelInterface *instance = nullptr;

  static ModelInterface *create(QQmlEngine *, QJSEngine *engine) {
    if (instance == nullptr) {
      qCritical() << "Model Interface never setup.";
    } else {
      QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
    }
    return instance;
  }

  static ModelInterface *init(QtNodes::AbstractGraphModel &_graphModel) {
    instance = new ModelInterface(_graphModel);
    return instance;
  };

  protected:
  ModelInterface(QtNodes::AbstractGraphModel &_graphModel)
      : QObject(nullptr), graphModel(_graphModel) {
    connect(&graphModel, SIGNAL(connectionCreated(ConnectionId)), this,
            SIGNAL(connectionCreated(ConnectionId)));
    connect(&graphModel, SIGNAL(connectionDeleted(ConnectionId)), this,
            SIGNAL(connectionDeleted(ConnectionId)));
    connect(&graphModel, SIGNAL(nodeCreated(NodeId)), this, SIGNAL(nodeCreated(NodeId)));
    connect(&graphModel, SIGNAL(nodeDeleted(NodeId)), this, SIGNAL(nodeDeleted(NodeId)));
    connect(&graphModel, SIGNAL(nodeUpdated(NodeId)), this, SIGNAL(nodeUpdated(NodeId)));
    connect(&graphModel, SIGNAL(nodeFlagsUpdated(NodeId)), this, SIGNAL(nodeFlagsUpdated(NodeId)));
    connect(&graphModel, SIGNAL(nodePositionUpdated(NodeId)), this,
            SIGNAL(nodePositionUpdated(NodeId)));
    connect(&graphModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
  }

  public:
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
