#include "GraphModel.hpp"

GraphModel::GraphModel() : _nextNodeId{0} {}

std::unordered_set<NodeId> GraphModel::allNodeIds() const { return _nodeIds; }

std::unordered_set<ConnectionId> GraphModel::allConnectionIds(NodeId const nodeId) const {
  std::unordered_set<ConnectionId> result;

  std::copy_if(_connectivity.begin(), _connectivity.end(), std::inserter(result, std::end(result)),
               [&nodeId](ConnectionId const &cid) {
                 return cid.inNodeId == nodeId || cid.outNodeId == nodeId;
               });

  return result;
}

std::unordered_set<ConnectionId> GraphModel::connections(NodeId nodeId, PortType portType,
                                                         PortIndex portIndex) const {
  std::unordered_set<ConnectionId> result;

  std::copy_if(_connectivity.begin(), _connectivity.end(), std::inserter(result, std::end(result)),
               [&portType, &portIndex, &nodeId](ConnectionId const &cid) {
                 return (getNodeId(portType, cid) == nodeId &&
                         getPortIndex(portType, cid) == portIndex);
               });

  return result;
}

bool GraphModel::connectionExists(ConnectionId const connectionId) const {
  return (_connectivity.find(connectionId) != _connectivity.end());
}

NodeId GraphModel::addNode(QString const nodeType) {
  NodeId newId = newNodeId();
  // Create new node.
  _nodeIds.insert(newId);

  Q_EMIT nodeCreated(newId);

  return newId;
}

bool GraphModel::connectionPossible(ConnectionId const connectionId) const {
  return _connectivity.find(connectionId) == _connectivity.end();
}

void GraphModel::addConnection(ConnectionId const connectionId) {
  _connectivity.insert(connectionId);

  Q_EMIT connectionCreated(connectionId);
}

bool GraphModel::nodeExists(NodeId const nodeId) const {
  return (_nodeIds.find(nodeId) != _nodeIds.end());
}

QVariant GraphModel::nodeData(NodeId nodeId, NodeRole role) const {
  Q_UNUSED(nodeId);

  QVariant result;

  switch (role) {
  case NodeRole::Type:
    result = QString("Default Node Type");
    break;

  case NodeRole::Position:
    result = _nodeGeometryData[nodeId].pos;
    break;

  case NodeRole::Size:
    result = _nodeGeometryData[nodeId].size;
    break;

  case NodeRole::CaptionVisible:
    result = true;
    break;

  case NodeRole::Caption:
    result = QString("Node");
    break;

  case NodeRole::Style: {
    auto style = StyleCollection::nodeStyle();
    result = style.toJson().toVariantMap();
  } break;

  case NodeRole::InternalData:
    break;

  case NodeRole::InPortCount:
    result = 1u;
    break;

  case NodeRole::OutPortCount:
    result = 1u;
    break;

  case NodeRole::Widget:
    result = QVariant();
    break;
  case NodeRole::ValidationState:
  case NodeRole::LabelVisible:
  case NodeRole::ProcessingStatus:
  case NodeRole::ProgressValue:
  case NodeRole::Label:
  case NodeRole::LabelEditable:
    break;
  }

  return result;
}

bool GraphModel::setNodeData(NodeId nodeId, NodeRole role, QVariant value) {
  bool result = false;

  switch (role) {
  case NodeRole::Type:
    break;
  case NodeRole::Position: {
    _nodeGeometryData[nodeId].pos = value.value<QPointF>();

    Q_EMIT nodePositionUpdated(nodeId);

    result = true;
  } break;

  case NodeRole::Size: {
    _nodeGeometryData[nodeId].size = value.value<QSize>();
    result = true;
  } break;

  case NodeRole::CaptionVisible:
    break;

  case NodeRole::Caption:
    break;

  case NodeRole::Style:
    break;

  case NodeRole::InternalData:
    break;

  case NodeRole::InPortCount:
    break;

  case NodeRole::OutPortCount:
    break;

  case NodeRole::Widget:
    break;
  case NodeRole::ValidationState:
  case NodeRole::LabelVisible:
  case NodeRole::ProcessingStatus:
  case NodeRole::ProgressValue:
  case NodeRole::Label:
  case NodeRole::LabelEditable:
    break;
  }

  return result;
}

QVariant GraphModel::portData(NodeId nodeId, PortType portType, PortIndex portIndex,
                              PortRole role) const {
  switch (role) {
  case PortRole::Data:
    return {};
    break;

  case PortRole::DataType:
    return {};
    break;

  case PortRole::ConnectionPolicyRole:
    return QVariant::fromValue(ConnectionPolicy::One);
    break;

  case PortRole::CaptionVisible:
    return true;
    break;

  case PortRole::Caption:
    if (portType == PortType::In)
      return QString::fromUtf8("Port In");
    else
      return QString::fromUtf8("Port Out");

    break;
  }

  return {};
}

bool GraphModel::setPortData(NodeId nodeId, PortType portType, PortIndex portIndex,
                             QVariant const &value, PortRole role) {
  Q_UNUSED(nodeId);
  Q_UNUSED(portType);
  Q_UNUSED(portIndex);
  Q_UNUSED(value);
  Q_UNUSED(role);

  return false;
}

bool GraphModel::deleteConnection(ConnectionId const connectionId) {
  bool disconnected = false;

  auto it = _connectivity.find(connectionId);

  if (it != _connectivity.end()) {
    disconnected = true;

    _connectivity.erase(it);
  }

  if (disconnected)
    Q_EMIT connectionDeleted(connectionId);

  return disconnected;
}

bool GraphModel::deleteNode(NodeId const nodeId) {
  // Delete connections to this node first.
  auto connectionIds = allConnectionIds(nodeId);

  for (auto &cId : connectionIds) {
    deleteConnection(cId);
  }

  _nodeIds.erase(nodeId);
  _nodeGeometryData.erase(nodeId);

  Q_EMIT nodeDeleted(nodeId);

  return true;
}

QJsonObject GraphModel::saveNode(NodeId const nodeId) const {
  QJsonObject nodeJson;

  nodeJson["id"] = static_cast<qint64>(nodeId);

  {
    auto const pos = nodeData(nodeId, NodeRole::Position).value<QPointF>();

    QJsonObject posJson;
    posJson["x"] = pos.x();
    posJson["y"] = pos.y();
    nodeJson["position"] = posJson;
  }

  return nodeJson;
}

void GraphModel::loadNode(QJsonObject const &nodeJson) {
  NodeId restoredNodeId = static_cast<NodeId>(nodeJson["id"].toInt());

  // Next NodeId must be larger that any id existing in the graph
  _nextNodeId = std::max(_nextNodeId, restoredNodeId + 1);

  // Create new node.
  _nodeIds.insert(restoredNodeId);

  Q_EMIT nodeCreated(restoredNodeId);

  {
    QJsonObject posJson = nodeJson["position"].toObject();
    QPointF const pos(posJson["x"].toDouble(), posJson["y"].toDouble());

    setNodeData(restoredNodeId, NodeRole::Position, pos);
  }
}
