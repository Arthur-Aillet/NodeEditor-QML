#include "ModelInterface.hpp"
#include "QtNodes/internal/DefaultHorizontalNodeGeometry.hpp"

ModelInterface::ModelInterface(QtNodes::AbstractGraphModel &_graphModel)
    : QObject(nullptr), graphModel(_graphModel), undoStack(QUndoStack(this)),
      _nodeGeometry(std::make_unique<QtNodes::DefaultHorizontalNodeGeometry>(graphModel)) {
  connect(&graphModel, SIGNAL(connectionCreated(ConnectionId)), this,
          SLOT(forwardConnectionCreated(ConnectionId)));
  connect(&graphModel, SIGNAL(connectionDeleted(ConnectionId)), this,
          SLOT(forwardConnectionDeleted(ConnectionId)));
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
