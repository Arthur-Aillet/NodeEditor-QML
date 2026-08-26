#pragma once

#include "Definitions.hpp"

#include <QJsonObject>

#include <iostream>
#include <string>

namespace NodeEditor {
inline PortIndex getNodeId(PortSide portSide, ConnectionId connectionId) {
  NodeId id = InvalidNodeId;

  if (portSide == PortSide::Out) {
    id = connectionId.outNodeId;
  } else if (portSide == PortSide::In) {
    id = connectionId.inNodeId;
  }

  return id;
}

inline PortIndex getPortIndex(PortSide portSide, ConnectionId connectionId) {
  PortIndex index = InvalidPortIndex;

  if (portSide == PortSide::Out) {
    index = connectionId.outPortIndex;
  } else if (portSide == PortSide::In) {
    index = connectionId.inPortIndex;
  }

  return index;
}

inline bool isPortIndexValid(PortIndex index) { return index != InvalidPortIndex; }

/**
 * Creates a connection Id instance filled just on one side.
 */
inline ConnectionId makeIncompleteConnectionId(NodeId const connectedNodeId,
                                               PortSide const connectedPort,
                                               PortIndex const connectedPortIndex) {
  return (connectedPort == PortSide::In)
             ? ConnectionId{InvalidNodeId, InvalidPortIndex, connectedNodeId, connectedPortIndex}
             : ConnectionId{connectedNodeId, connectedPortIndex, InvalidNodeId, InvalidPortIndex};
}

/**
 * Turns a full connection Id into an incomplete one by removing the
 * data on the given side
 */
inline ConnectionId makeIncompleteConnectionId(ConnectionId connectionId,
                                               PortSide const portToDisconnect) {
  if (portToDisconnect == PortSide::Out) {
    connectionId.outNodeId = InvalidNodeId;
    connectionId.outPortIndex = InvalidPortIndex;
  } else {
    connectionId.inNodeId = InvalidNodeId;
    connectionId.inPortIndex = InvalidPortIndex;
  }

  return connectionId;
}

inline ConnectionId makeCompleteConnectionId(ConnectionId incompleteConnectionId,
                                             NodeId const nodeId, PortIndex const portIndex) {
  if (incompleteConnectionId.outNodeId == InvalidNodeId) {
    incompleteConnectionId.outNodeId = nodeId;
    incompleteConnectionId.outPortIndex = portIndex;
  } else {
    incompleteConnectionId.inNodeId = nodeId;
    incompleteConnectionId.inPortIndex = portIndex;
  }

  return incompleteConnectionId;
}

inline std::ostream &operator<<(std::ostream &ostr, ConnectionId const connectionId) {
  ostr << "(" << connectionId.outNodeId << ", "
       << (isPortIndexValid(connectionId.outPortIndex) ? std::to_string(connectionId.outPortIndex)
                                                       : "INVALID")
       << ", " << connectionId.inNodeId << ", "
       << (isPortIndexValid(connectionId.inPortIndex) ? std::to_string(connectionId.inPortIndex)
                                                      : "INVALID")
       << ")" << std::endl;

  return ostr;
}

inline QJsonObject toJson(ConnectionId const &connId) {
  QJsonObject connJson;

  connJson["outNodeId"] = static_cast<qint64>(connId.outNodeId);
  connJson["outPortIndex"] = static_cast<qint64>(connId.outPortIndex);
  connJson["inNodeId"] = static_cast<qint64>(connId.inNodeId);
  connJson["inPortIndex"] = static_cast<qint64>(connId.inPortIndex);

  return connJson;
}

inline ConnectionId fromJson(QJsonObject const &connJson) {
  // Support both "inNodeId" (correct) and "intNodeId" (legacy typo) for backward compatibility
  NodeId inNodeId = InvalidNodeId;
  if (connJson.contains("inNodeId")) {
    inNodeId = static_cast<NodeId>(connJson["inNodeId"].toInt(InvalidNodeId));
  } else if (connJson.contains("intNodeId")) {
    inNodeId = static_cast<NodeId>(connJson["intNodeId"].toInt(InvalidNodeId));
  }

  ConnectionId connId{static_cast<NodeId>(connJson["outNodeId"].toInt(InvalidNodeId)),
                      static_cast<PortIndex>(connJson["outPortIndex"].toInt(InvalidPortIndex)),
                      inNodeId,
                      static_cast<PortIndex>(connJson["inPortIndex"].toInt(InvalidPortIndex))};

  return connId;
}
}; // namespace NodeEditor