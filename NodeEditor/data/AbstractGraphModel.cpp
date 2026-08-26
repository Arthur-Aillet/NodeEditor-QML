#include "AbstractGraphModel.hpp"
#include "ConnectionIdUtils.hpp"

using namespace NodeEditor;

void AbstractGraphModel::portsAboutToBeDeleted(NodeId const nodeId, PortSide const portSide,
                                               PortIndex const first, PortIndex const last) {
  _shiftedByDynamicPortsConnections.clear();

  auto portCountRole = portSide == PortSide::In ? NodeRole::InPortCount : NodeRole::OutPortCount;

  unsigned int portCount = nodeData(nodeId, portCountRole).toUInt();

  if (first > portCount - 1)
    return;

  if (last < first)
    return;

  auto clampedLast = std::min(last, portCount - 1);

  for (PortIndex portIndex = first; portIndex <= clampedLast; ++portIndex) {
    QSet<ConnectionId> conns = connections(nodeId, portSide, portIndex);

    for (auto connectionId : conns) {
      deleteConnection(connectionId);
    }
  }

  size_t const nRemovedPorts = clampedLast - first + 1;

  for (PortIndex portIndex = clampedLast + 1; portIndex < portCount; ++portIndex) {
    QSet<ConnectionId> conns = connections(nodeId, portSide, portIndex);

    for (auto connectionId : conns) {
      // Erases the information about the port on one side;
      auto c = makeIncompleteConnectionId(connectionId, portSide);

      c = makeCompleteConnectionId(c, nodeId, portIndex - static_cast<PortIndex>(nRemovedPorts));

      _shiftedByDynamicPortsConnections.push_back(c);

      deleteConnection(connectionId);
    }
  }
}

void AbstractGraphModel::portsDeleted(NodeId const nodeId, PortSide const portSide) {
  for (auto const connectionId : _shiftedByDynamicPortsConnections) {
    addConnection(connectionId);
  }

  _shiftedByDynamicPortsConnections.clear();
  emit nodePortsUpdated(nodeId, portSide);
}

void AbstractGraphModel::portsAboutToBeInserted(NodeId const nodeId, PortSide const portSide,
                                                PortIndex const first, PortIndex const last) {
  _shiftedByDynamicPortsConnections.clear();

  auto portCountRole = portSide == PortSide::In ? NodeRole::InPortCount : NodeRole::OutPortCount;

  unsigned int portCount = nodeData(nodeId, portCountRole).toUInt();

  if (first > portCount)
    return;

  if (last < first)
    return;

  size_t const nNewPorts = last - first + 1;

  for (PortIndex portIndex = first; portIndex < portCount; ++portIndex) {
    QSet<ConnectionId> conns = connections(nodeId, portSide, portIndex);

    for (auto connectionId : conns) {
      // Erases the information about the port on one side;
      auto c = makeIncompleteConnectionId(connectionId, portSide);

      c = makeCompleteConnectionId(c, nodeId, portIndex + static_cast<PortIndex>(nNewPorts));

      _shiftedByDynamicPortsConnections.push_back(c);

      deleteConnection(connectionId);
    }
  }
}

void AbstractGraphModel::portsInserted(NodeId const nodeId, PortSide const portSide) {
  for (auto const connectionId : _shiftedByDynamicPortsConnections) {
    addConnection(connectionId);
  }

  _shiftedByDynamicPortsConnections.clear();
  emit nodePortsUpdated(nodeId, portSide);
}
