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

class DataFlowModelInterface : public ModelInterface {
  Q_OBJECT
  QML_SINGLETON
  QML_ELEMENT

  public:
  QtNodes::DataFlowGraphModel &graphModel;

  Q_INVOKABLE bool test() { return true; }

  static DataFlowModelInterface *create(QQmlEngine *, QJSEngine *engine) {
    if (instance == nullptr) {
      qCritical() << "Data Flow Model Interface never setup.";
    } else {
      QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
    }
    return dynamic_cast<DataFlowModelInterface *>(instance);
  }

  static DataFlowModelInterface *init(QtNodes::DataFlowGraphModel &_graphModel) {
    instance = new DataFlowModelInterface(_graphModel);
    return dynamic_cast<DataFlowModelInterface *>(instance);
  };

  protected:
  DataFlowModelInterface(QtNodes::DataFlowGraphModel &_graphModel)
      : ModelInterface(_graphModel), graphModel(_graphModel) {}
};
