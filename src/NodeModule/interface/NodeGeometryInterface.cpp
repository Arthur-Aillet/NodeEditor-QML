#include "NodeGeometryInterface.hpp"

QRectF NodeGeometryInterface::boundingRect(NodeId const nodeId) const {
  return nodeGeometry->boundingRect(nodeId);
}

QSize NodeGeometryInterface::size(NodeId const nodeId) const { return nodeGeometry->size(nodeId); }

void NodeGeometryInterface::recomputeSize(NodeId const nodeId) const {
  return nodeGeometry->recomputeSize(nodeId);
}

QPointF NodeGeometryInterface::portPosition(NodeId const nodeId, PortType const portType,
                                            PortIndex const index) const {
  return nodeGeometry->portPosition(nodeId, portType, index);
}

QPointF NodeGeometryInterface::portScenePosition(NodeId const nodeId, PortType const portType,
                                                 PortIndex const index, QTransform const &t) const {
  return nodeGeometry->portScenePosition(nodeId, portType, index, t);
}

QPointF NodeGeometryInterface::portTextPosition(NodeId const nodeId, PortType const portType,
                                                PortIndex const portIndex) const {
  return nodeGeometry->portTextPosition(nodeId, portType, portIndex);
}

QPointF NodeGeometryInterface::captionPosition(NodeId const nodeId) const {
  return nodeGeometry->captionPosition(nodeId);
}

QRectF NodeGeometryInterface::captionRect(NodeId const nodeId) const {
  return nodeGeometry->captionRect(nodeId);
}

QPointF NodeGeometryInterface::labelPosition(NodeId const nodeId) const {
  return nodeGeometry->labelPosition(nodeId);
}

QRectF NodeGeometryInterface::labelRect(NodeId const nodeId) const {
  return nodeGeometry->labelRect(nodeId);
}

QPointF NodeGeometryInterface::widgetPosition(NodeId const nodeId) const {
  return nodeGeometry->widgetPosition(nodeId);
}

PortIndex NodeGeometryInterface::checkPortHit(NodeId const nodeId, PortType const portType,
                                              QPointF const nodePoint) const {
  return nodeGeometry->checkPortHit(nodeId, portType, nodePoint);
}

QRect NodeGeometryInterface::resizeHandleRect(NodeId const nodeId) const {
  return nodeGeometry->resizeHandleRect(nodeId);
}

int NodeGeometryInterface::getPortSpacing() { return nodeGeometry->getPortSpacing(); }

NodeGeometryInterface::NodeGeometryInterface(AbstractNodeGeometry *_nodeGeometry)
    : nodeGeometry(_nodeGeometry) {}

bool NodeGeometryInterface::operator==(const NodeGeometryInterface &other) {
  return nodeGeometry == other.nodeGeometry;
}
