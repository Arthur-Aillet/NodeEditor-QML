#include "ModelInterface.hpp"

ModelInterface::ModelInterface(AbstractGraphModel &_graphModel)
    : QObject(nullptr), graphModel(_graphModel), undoStack(QUndoStack(this)) {}

ModelInterface *ModelInterface::create(QQmlEngine *, QJSEngine *engine) {
  if (instance == nullptr) {
    qCritical() << "Model Interface never setup.";
  } else {
    QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
  }
  return instance;
}

ModelInterface *ModelInterface::init(AbstractGraphModel &_graphModel) {
  instance = new ModelInterface(_graphModel);
  return instance;
};

void ModelInterface::createConnection(ConnectionId id) {
  undoStack.push(new ConnectCommand(this, id));
}

void ModelInterface::deleteConnection(ConnectionId id) {
  undoStack.push(new DisconnectCommand(this, id));
}

void ModelInterface::createNode(QString const nodeType, QPoint const &scenePos) {
  undoStack.push(new CreateCommand(this, nodeType, scenePos));
}

bool ModelInterface::deleteNode(const NodeId nodeId) {
  // TODO: restore DeleteNodeCommand
  return graphModel.deleteNode(nodeId);
}
