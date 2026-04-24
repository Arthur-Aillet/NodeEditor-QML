#include "ModelInterface.hpp"
#include <qqmlengine.h>

ModelInterface::ModelInterface(QtNodes::AbstractGraphModel &_graphModel)
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

ModelInterface *ModelInterface::create(QQmlEngine *, QJSEngine *engine) {
  if (instance == nullptr) {
    qCritical() << "Model Interface never setup.";
  } else {
    QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
  }
  return instance;
}

ModelInterface *ModelInterface::init(QtNodes::AbstractGraphModel &_graphModel) {
  instance = new ModelInterface(_graphModel);
  return instance;
};
