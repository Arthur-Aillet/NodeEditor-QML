#pragma once

#include "AbstractNodeGeometry.hpp"
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class NodeGeometryInterface {

  Q_GADGET
  QML_VALUE_TYPE(nodeGeometryInterface)

  AbstractNodeGeometry *nodeGeometry;

  public:
  Q_INVOKABLE QRectF boundingRect(NodeId const nodeId) const;
  Q_INVOKABLE QSize size(NodeId const nodeId) const;
  Q_INVOKABLE void recomputeSize(NodeId const nodeId) const;
  Q_INVOKABLE QPointF portPosition(NodeId const nodeId, PortType const portType,
                                   PortIndex const index) const;
  Q_INVOKABLE QPointF portScenePosition(NodeId const nodeId, PortType const portType,
                                        PortIndex const index, QTransform const &t) const;
  Q_INVOKABLE QPointF portTextPosition(NodeId const nodeId, PortType const portType,
                                       PortIndex const portIndex) const;
  Q_INVOKABLE QPointF captionPosition(NodeId const nodeId) const;
  Q_INVOKABLE QRectF captionRect(NodeId const nodeId) const;
  Q_INVOKABLE QPointF labelPosition(NodeId const nodeId) const;
  Q_INVOKABLE QRectF labelRect(NodeId const nodeId) const;
  Q_INVOKABLE QPointF widgetPosition(NodeId const nodeId) const;
  Q_INVOKABLE PortIndex checkPortHit(NodeId const nodeId, PortType const portType,
                                     QPointF const nodePoint) const;
  Q_INVOKABLE QRect resizeHandleRect(NodeId const nodeId) const;
  Q_INVOKABLE int getPortSpacing();

  NodeGeometryInterface(AbstractNodeGeometry *_nodeGeometry = nullptr);
  bool operator==(const NodeGeometryInterface &other);
};