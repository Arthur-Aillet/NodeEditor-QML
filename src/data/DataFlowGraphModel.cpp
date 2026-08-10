#include "DataFlowGraphModel.hpp"
#include "ConnectionIdUtils.hpp"
#include "Definitions.hpp"
#include "NodeData.hpp"

#include <stack>

DataFlowGraphModel::DataFlowGraphModel(std::shared_ptr<NodeDelegateModelRegistry> registry,
                                       QQmlEngine *engine)
    : _registry(std::move(registry)), _engine(engine) {}

QSet<NodeId> DataFlowGraphModel::allNodeIds() const {
  QSet<NodeId> nodeIds;
  for_each(_models.begin(), _models.end(), [&nodeIds](auto const &p) { nodeIds.insert(p.first); });

  return nodeIds;
}

QSet<ConnectionId> DataFlowGraphModel::allConnectionIds(NodeId const nodeId) const {
  QSet<ConnectionId> result;

  std::copy_if(_connectivity.begin(), _connectivity.end(), std::inserter(result, std::end(result)),
               [&nodeId](ConnectionId const &cid) {
                 return cid.inNodeId == nodeId || cid.outNodeId == nodeId;
               });

  return result;
}

QSet<ConnectionId> DataFlowGraphModel::connections(NodeId nodeId, PortType portType,
                                                   PortIndex portIndex) const {
  QSet<ConnectionId> result;

  std::copy_if(_connectivity.begin(), _connectivity.end(), std::inserter(result, std::end(result)),
               [&portType, &portIndex, &nodeId](ConnectionId const &cid) {
                 return (getNodeId(portType, cid) == nodeId &&
                         getPortIndex(portType, cid) == portIndex);
               });

  return result;
}

bool DataFlowGraphModel::connectionExists(ConnectionId const connectionId) const {
  return (_connectivity.find(connectionId) != _connectivity.end());
}

NodeId DataFlowGraphModel::addNode(QString const nodeType) {
  std::unique_ptr<NodeDelegateModel> model = _registry->create(nodeType);

  if (model) {
    NodeId newId = newNodeId();

    connectNode(model.get(), newId);
    _models[newId] = std::move(model);

    _labels[newId] = _models[newId]->label();
    _labelsVisible[newId] = _models[newId]->labelVisible();

    emit nodeCreated(newId);

    return newId;
  }

  return InvalidNodeId;
}

bool DataFlowGraphModel::connectionPossible(ConnectionId const connectionId) const {
  // Check if nodes exist
  if (!nodeExists(connectionId.outNodeId) || !nodeExists(connectionId.inNodeId)) {
    return false;
  }

  // Check port bounds, i.e. that we do not connect non-existing port numbers
  auto checkPortBounds = [&](PortType const portType) {
    NodeId const nodeId = getNodeId(portType, connectionId);
    auto portCountRole =
        (portType == PortType::Out) ? NodeRole::OutPortCount : NodeRole::InPortCount;

    std::size_t const portCount = nodeData(nodeId, portCountRole).toUInt();

    return getPortIndex(portType, connectionId) < portCount;
  };

  auto getDataType = [&](PortType const portType) -> NodeDataType {
    return portData(getNodeId(portType, connectionId), portType,
                    getPortIndex(portType, connectionId), PortRole::DataType)
        .value<NodeDataType>();
  };

  auto portVacant = [&](PortType const portType) {
    NodeId const nodeId = getNodeId(portType, connectionId);
    PortIndex const portIndex = getPortIndex(portType, connectionId);
    auto policy = portData(nodeId, portType, portIndex, PortRole::ConnectionPolicyRole)
                      .value<ConnectionPolicy>();

    if (policy != ConnectionPolicy::One)
      return true;
    return connections(nodeId, portType, portIndex).empty();
  };

  bool const basicChecks =
      getDataType(PortType::Out).compatibleTypes.contains(getDataType(PortType::In).id) &&
      portVacant(PortType::Out) && portVacant(PortType::In) && checkPortBounds(PortType::Out) &&
      checkPortBounds(PortType::In);

  // In data-flow mode (this class) it's important to forbid graph loops.
  // We perform depth-first graph traversal starting from the "Input" port of
  // the given connection. We should never encounter the starting "Out" node.

  auto hasLoops = [this, &connectionId]() -> bool {
    std::stack<NodeId> filo;
    filo.push(connectionId.inNodeId);

    while (!filo.empty()) {
      auto id = filo.top();
      filo.pop();

      if (id == connectionId.outNodeId) { // LOOP!
        return true;
      }

      // Add out-connections to continue interations
      std::size_t const nOutPorts = nodeData(id, NodeRole::OutPortCount).toUInt();

      for (PortIndex index = 0; index < nOutPorts; ++index) {
        auto const &outConnectionIds = connections(id, PortType::Out, index);

        for (auto cid : outConnectionIds) {
          filo.push(cid.inNodeId);
        }
      }
    }

    return false;
  };

  return basicChecks && (loopsEnabled() || !hasLoops());
}

void DataFlowGraphModel::addConnection(ConnectionId const connectionId) {
  auto removeReplaceConnections = [&](PortType const portType) {
    NodeId const nodeId = getNodeId(portType, connectionId);
    PortIndex const portIndex = getPortIndex(portType, connectionId);
    auto policy = portData(nodeId, portType, portIndex, PortRole::ConnectionPolicyRole)
                      .value<ConnectionPolicy>();
    if (policy == ConnectionPolicy::Replace) {
      for (auto &con : connections(nodeId, portType, portIndex)) {
        deleteConnection(con);
      }
    }
  };

  removeReplaceConnections(PortType::In);
  removeReplaceConnections(PortType::Out);

  _connectivity.insert(connectionId);

  sendConnectionCreation(connectionId);

  QVariant const portDataToPropagate =
      portData(connectionId.outNodeId, PortType::Out, connectionId.outPortIndex, PortRole::Data);

  setPortData(connectionId.inNodeId, PortType::In, connectionId.inPortIndex, portDataToPropagate,
              PortRole::Data);
}

void DataFlowGraphModel::sendConnectionCreation(ConnectionId const connectionId) {
  Q_EMIT connectionCreated(connectionId);

  auto iti = _models.find(connectionId.inNodeId);
  auto ito = _models.find(connectionId.outNodeId);
  if (iti != _models.end() && ito != _models.end()) {
    auto &modeli = iti->second;
    auto &modelo = ito->second;
    modeli->inputConnectionCreated(connectionId);
    modelo->outputConnectionCreated(connectionId);
  }
}

void DataFlowGraphModel::sendConnectionDeletion(ConnectionId const connectionId) {
  Q_EMIT connectionDeleted(connectionId);

  auto iti = _models.find(connectionId.inNodeId);
  auto ito = _models.find(connectionId.outNodeId);
  if (iti != _models.end() && ito != _models.end()) {
    auto &modeli = iti->second;
    auto &modelo = ito->second;
    modeli->inputConnectionDeleted(connectionId);
    modelo->outputConnectionDeleted(connectionId);
  }
}

bool DataFlowGraphModel::nodeExists(NodeId const nodeId) const {
  return (_models.find(nodeId) != _models.end());
}

QVariant DataFlowGraphModel::nodeData(NodeId nodeId, NodeRole role) const {
  QVariant result;

  auto it = _models.find(nodeId);
  if (it == _models.end())
    return result;

  auto &model = it->second;

  switch (role) {
  case NodeRole::Type:
    result = model->name();
    break;

  case NodeRole::Position:
    result = _nodeGeometryData[nodeId].pos;
    break;

  case NodeRole::Size:
    result = _nodeGeometryData[nodeId].size;
    break;

  case NodeRole::CaptionVisible:
    result = model->captionVisible();
    break;

  case NodeRole::Caption:
    result = model->caption();
    break;

  case NodeRole::Style: {
    auto style = model->nodeStyle();
    result = style.toJson().toVariantMap();
  } break;

  case NodeRole::InternalData: {
    QJsonObject nodeJson;

    nodeJson["internal-data"] = model->save();

    result = nodeJson.toVariantMap();
    break;
  }

  case NodeRole::InPortCount:
    result = model->nPorts(PortType::In);
    break;

  case NodeRole::OutPortCount:
    result = model->nPorts(PortType::Out);
    break;

  case NodeRole::ValidationState: {
    auto validationState = model->validationState();
    result = QVariant::fromValue(validationState);
  } break;

  case NodeRole::LabelVisible: {
    auto const labelVisibleIt = _labelsVisible.find(nodeId);
    result =
        (labelVisibleIt != _labelsVisible.end()) ? labelVisibleIt->second : model->labelVisible();
  } break;

  case NodeRole::Label: {
    auto const labelIt = _labels.find(nodeId);
    result = (labelIt != _labels.end()) ? labelIt->second : model->label();
  } break;

  case NodeRole::LabelEditable:
    result = model->labelEditable();
    break;

  case NodeRole::ProcessingStatus: {
    auto processingStatus = model->processingStatus();
    result = QVariant::fromValue(processingStatus);
  } break;

  case NodeRole::ProgressValue:
    result = model->progressValue();
    break;

  case NodeRole::Flags:
    result = QVariant::fromValue(model->flags());
    break;
  }

  return result;
}

bool DataFlowGraphModel::setNodeData(NodeId nodeId, NodeRole role, QVariant value) {
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

  case NodeRole::ValidationState: {
    if (value.canConvert<NodeValidationState>()) {
      auto state = value.value<NodeValidationState>();
      if (auto node = delegateModel<NodeDelegateModel>(nodeId); node != nullptr) {
        node->setValidationState(state);
      }
    }
    Q_EMIT nodeUpdated(nodeId);
  } break;

  case NodeRole::ProcessingStatus: {
    if (value.canConvert<NodeProcessingStatus>()) {
      auto status = value.value<NodeProcessingStatus>();
      if (auto node = delegateModel<NodeDelegateModel>(nodeId); node != nullptr) {
        node->setNodeProcessingStatus(status);
      }
    }
    Q_EMIT nodeUpdated(nodeId);
  } break;

  case NodeRole::LabelVisible: {
    _labelsVisible[nodeId] = value.toBool();
    Q_EMIT nodeUpdated(nodeId);
    result = true;
  } break;

  case NodeRole::Label: {
    _labels[nodeId] = value.toString();
    Q_EMIT nodeUpdated(nodeId);
    result = true;
  } break;

  case NodeRole::LabelEditable:
    break;

  case NodeRole::ProgressValue:
    break;

  case NodeRole::Flags:
    if (value.canConvert<NodeFlags>()) {
      auto flags = value.value<NodeFlags>();
      if (auto node = delegateModel<NodeDelegateModel>(nodeId); node != nullptr) {
        node->setFlags(flags);
      }
    }
    Q_EMIT nodeFlagsUpdated(nodeId);
    result = true;
    break;
  }

  return result;
}

QVariant DataFlowGraphModel::portData(NodeId nodeId, PortType portType, PortIndex portIndex,
                                      PortRole role) const {
  QVariant result;

  auto it = _models.find(nodeId);
  if (it == _models.end())
    return result;

  auto &model = it->second;

  switch (role) {
  case PortRole::Data:
    if (portType == PortType::Out) {
      result = QVariant::fromValue(model->outData(portIndex));
    }
    break;

  case PortRole::DataType:
    result = QVariant::fromValue(model->dataType(portType, portIndex));
    break;

  case PortRole::ConnectionPolicyRole:
    result = QVariant::fromValue(model->portConnectionPolicy(portType, portIndex));
    break;

  case PortRole::PortCaptionVisible:
    result = model->portCaptionVisible(portType, portIndex);
    break;

  case PortRole::PortCaption:
    result = model->portCaption(portType, portIndex);

    break;
  }

  return result;
}

bool DataFlowGraphModel::setPortData(NodeId nodeId, PortType portType, PortIndex portIndex,
                                     QVariant const &value, PortRole role) {
  Q_UNUSED(nodeId);

  QVariant result;

  auto it = _models.find(nodeId);
  if (it == _models.end())
    return false;

  auto &model = it->second;

  switch (role) {
  case PortRole::Data:
    if (portType == PortType::In) {
      model->setInData(value.value<std::shared_ptr<NodeData>>(), portIndex);

      // Triggers repainting on the scene.
      Q_EMIT inPortDataWasSet(nodeId, portType, portIndex);
    }
    break;

  default:
    break;
  }

  return false;
}

bool DataFlowGraphModel::deleteConnection(ConnectionId const connectionId) {
  bool disconnected = false;

  auto it = _connectivity.find(connectionId);

  if (it != _connectivity.end()) {
    disconnected = true;

    _connectivity.erase(it);
  }

  if (disconnected) {
    sendConnectionDeletion(connectionId);

    propagateEmptyDataTo(getNodeId(PortType::In, connectionId),
                         getPortIndex(PortType::In, connectionId));
  }

  return disconnected;
}

bool DataFlowGraphModel::deleteNode(NodeId const nodeId) {
  // Delete connections to this node first.
  auto connectionIds = allConnectionIds(nodeId);
  for (auto &cId : connectionIds) {
    deleteConnection(cId);
  }

  _nodeGeometryData.erase(nodeId);
  _labels.erase(nodeId);
  _labelsVisible.erase(nodeId);
  _models.erase(nodeId);

  Q_EMIT nodeDeleted(nodeId);

  return true;
}

bool DataFlowGraphModel::clear() {
  for (auto &nodeId : allNodeIds()) {
    if (!deleteNode(nodeId)) {
      return false;
    }
  }
  return true;
}

QJsonObject DataFlowGraphModel::saveNode(NodeId const nodeId) const {
  QJsonObject nodeJson;

  auto const modelIt = _models.find(nodeId);
  if (modelIt == _models.end()) {
    return nodeJson;
  }

  auto const &model = modelIt->second;

  nodeJson["id"] = static_cast<qint64>(nodeId);
  nodeJson["internal-data"] = model->save();
  nodeJson["model-name"] = model->name();

  auto const labelIt = _labels.find(nodeId);
  nodeJson["label"] = (labelIt != _labels.end()) ? labelIt->second : model->label();

  auto const labelVisibleIt = _labelsVisible.find(nodeId);
  nodeJson["labelVisible"] =
      (labelVisibleIt != _labelsVisible.end()) ? labelVisibleIt->second : model->labelVisible();

  QPointF const pos = nodeData(nodeId, NodeRole::Position).value<QPointF>();

  QJsonObject posJson;
  posJson["x"] = pos.x();
  posJson["y"] = pos.y();
  nodeJson["position"] = posJson;

  auto flags = nodeData(nodeId, NodeRole::Flags).toInt();
  if (flags != 0)
    nodeJson["flags"] = flags;

  return nodeJson;
}

QJsonObject DataFlowGraphModel::save() const {
  QJsonObject sceneJson;

  QJsonArray nodesJsonArray;
  for (auto const nodeId : allNodeIds()) {
    nodesJsonArray.append(saveNode(nodeId));
  }
  sceneJson["nodes"] = nodesJsonArray;

  QJsonArray connJsonArray;
  for (auto const &cid : _connectivity) {
    connJsonArray.append(toJson(cid));
  }
  sceneJson["connections"] = connJsonArray;

  return sceneJson;
}

NodeId DataFlowGraphModel::loadNode(QJsonObject const &nodeJson) {
  QString delegateModelName = nodeJson["model-name"].toString();

  std::unique_ptr<NodeDelegateModel> model = _registry->create(delegateModelName);

  if (model) {
    NodeId nodeId = nodeJson["id"].toInt();
    const auto &nodesIds = allNodeIds();
    if (nodesIds.contains(nodeId)) {
      nodeId = newNodeId();
    }

    connectNode(model.get(), nodeId);
    _models[nodeId] = std::move(model);

    auto *restoredModel = _models[nodeId].get();
    restoredModel->load(nodeJson["internal-data"].toObject());

    _labels[nodeId] = nodeJson["label"].toString(restoredModel->label());
    _labelsVisible[nodeId] = nodeJson.contains("labelVisible") ? nodeJson["labelVisible"].toBool()
                                                               : restoredModel->labelVisible();
    if (!nodeJson["flags"].isUndefined()) {
      setNodeData(nodeId, NodeRole::Flags, nodeJson["flags"].toInt());
    }

    emit nodeCreated(nodeId);

    QJsonObject posJson = nodeJson["position"].toObject();
    QPointF const pos(posJson["x"].toDouble(), posJson["y"].toDouble());

    setNodeData(nodeId, NodeRole::Position, pos);

    return nodeId;
  } else {
    throw std::logic_error(std::string("No registered model with name ") +
                           delegateModelName.toLocal8Bit().data());
  }
  return InvalidNodeId;
}

void DataFlowGraphModel::load(QJsonObject const &jsonDocument) {
  QJsonArray nodesJsonArray = jsonDocument["nodes"].toArray();

  std::unordered_map<NodeId, NodeId> idConversionMap;

  for (QJsonValueRef nodeJson : nodesJsonArray) {
    QJsonObject const nodeJsonObject = nodeJson.toObject();
    NodeId originalNodeId = nodeJsonObject["id"].toInt();
    NodeId newNodeId = loadNode(nodeJsonObject);
    idConversionMap.insert({originalNodeId, newNodeId});
  }

  QJsonArray connectionJsonArray = jsonDocument["connections"].toArray();

  for (QJsonValueRef connection : connectionJsonArray) {
    QJsonObject connJson = connection.toObject();

    ConnectionId connId = fromJson(connJson);

    // Connect with the new node Ids if their nodes have changed Id
    connId.inNodeId = idConversionMap[connId.inNodeId];
    connId.outNodeId = idConversionMap[connId.outNodeId];
    // Restore the connection
    addConnection(connId);
  }
}

void DataFlowGraphModel::requestComponent(NodeId nodeId, QQuickItem *container) {
  _models[nodeId]->createComponent(container, _engine);
}

void DataFlowGraphModel::onOutPortDataUpdated(NodeId const nodeId, PortIndex const portIndex) {
  QSet<ConnectionId> const &connected = connections(nodeId, PortType::Out, portIndex);

  QVariant const portDataToPropagate = portData(nodeId, PortType::Out, portIndex, PortRole::Data);

  for (auto const &cn : connected) {
    setPortData(cn.inNodeId, PortType::In, cn.inPortIndex, portDataToPropagate, PortRole::Data);
  }
}

void DataFlowGraphModel::propagateEmptyDataTo(NodeId const nodeId, PortIndex const portIndex) {
  QVariant emptyData{};

  setPortData(nodeId, PortType::In, portIndex, emptyData, PortRole::Data);
}

NodeId DataFlowGraphModel::newNodeId() {
  NodeId i = 0;
  for (; i != _models.size(); i++) {
    if (_models.count(i) == 0) {
      return i;
    }
  }
  return i;
}

void DataFlowGraphModel::connectNode(NodeDelegateModel *model, NodeId nodeId) {
  connect(model, &NodeDelegateModel::dataUpdated,
          [nodeId, this](PortIndex const portIndex) { onOutPortDataUpdated(nodeId, portIndex); });

  connect(model, &NodeDelegateModel::dataInvalidated,
          [nodeId, this](PortIndex const portIndex) { onOutPortDataUpdated(nodeId, portIndex); });

  connect(model, &NodeDelegateModel::portsAboutToBeDeleted,
          [nodeId, this](PortType const portType, PortIndex const first, PortIndex const last) {
            portsAboutToBeDeleted(nodeId, portType, first, last);
          });

  connect(model, &NodeDelegateModel::portsDeleted,
          [nodeId, this](PortType const portType) { portsDeleted(nodeId, portType); });

  connect(model, &NodeDelegateModel::portsAboutToBeInserted,
          [nodeId, this](PortType const portType, PortIndex const first, PortIndex const last) {
            portsAboutToBeInserted(nodeId, portType, first, last);
          });

  connect(model, &NodeDelegateModel::portsInserted,
          [nodeId, this](PortType const portType) { portsInserted(nodeId, portType); });

  connect(model, &NodeDelegateModel::portsNameChanged,
          [nodeId, this](PortType const portType) { nodePortsUpdated(nodeId, portType); });
}