#include "ModelInterface.hpp"

ModelInterface::ModelInterface(AbstractGraphModel &_graphModel)
    : QObject(nullptr), graphModel(_graphModel), undoStack(QUndoStack(this)) {
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

ModelInterface *ModelInterface::init(AbstractGraphModel &_graphModel) {
  instance = new ModelInterface(_graphModel);
  return instance;
};

QVariant ModelInterface::nodeData(NodeId nodeId, NodeRole role) {
  return graphModel.nodeData(nodeId, role);
};

QVariant ModelInterface::portData(NodeId nodeId, PortType portType, PortIndex index,
                                  PortRole role) {
  return graphModel.portData(nodeId, portType, index, role);
};

bool ModelInterface::setNodeData(NodeId nodeId, NodeRole role, QVariant value) {
  return graphModel.setNodeData(nodeId, role, value);
}

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
