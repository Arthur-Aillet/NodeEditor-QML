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
    connect(&graphModel, SIGNAL(nodeCreated(NodeId)), this, SIGNAL(nodeCreated(NodeId)));
  }

  public:
  Q_SIGNALS:
  void nodeCreated(NodeId const nodeId);
};
