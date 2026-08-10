#include "ModelInterface.hpp"
#include "Definitions.hpp"

ModelInterface::ModelInterface(AbstractGraphModel *_graphModel)
    : QObject(nullptr), graphModel(_graphModel), undoStack(QUndoStack(this)) {}

ModelInterface *ModelInterface::create(QQmlEngine *, QJSEngine *engine) {
  if (instance == nullptr) {
    qCritical() << "Model Interface never setup.";
  } else {
    QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
  }
  return instance;
}

ModelInterface *ModelInterface::init(AbstractGraphModel *_graphModel) {
  instance = new ModelInterface(_graphModel);
  return instance;
};

void ModelInterface::createConnection(ConnectionId id) {
  undoStack.push(new ConnectCommand(this, id));
}

void ModelInterface::deleteConnection(ConnectionId id) {
  undoStack.push(new DisconnectCommand(this, id));
}

NodeId ModelInterface::createNode(QString const nodeType, QPoint const &scenePos) {
  auto command = new CreateCommand(this, nodeType, scenePos);
  undoStack.push(command);
  return command->nodeId;
}
