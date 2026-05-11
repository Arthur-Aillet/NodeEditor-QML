#pragma once

#include "Definitions.hpp"

#include <QRectF>
#include <QSize>
#include <QTransform>
#include <QtCore/QObject>

#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class AbstractGraphModel;

class AbstractNodeGeometry : public QObject {
  Q_OBJECT
  QML_INTERFACE

  public:
  AbstractNodeGeometry(AbstractGraphModel &);
  virtual ~AbstractNodeGeometry() {}

  /**
   * The node's size plus some additional margin around it to account for drawing
   * effects (for example shadows) or node's parts outside the size rectangle
   * (for example port points).
   */
  Q_INVOKABLE virtual QRectF boundingRect(NodeId const nodeId) const = 0;

  /// A direct rectangle defining the borders of the node's rectangle.
  Q_INVOKABLE virtual QSize size(NodeId const nodeId) const = 0;

  /**
   * The function is triggeren when a number of ports is changed or when an
   * embedded widget needs an update.
   */
  Q_INVOKABLE virtual void recomputeSize(NodeId const nodeId) const = 0;

  /// Port position in node's coordinate system.
  Q_INVOKABLE virtual QPointF portPosition(NodeId const nodeId, PortType const portType,
                                           PortIndex const index) const = 0;

  /// A convenience function using the `portPosition` and a given transformation.
  Q_INVOKABLE virtual QPointF portScenePosition(NodeId const nodeId, PortType const portType,
                                                PortIndex const index, QTransform const &t) const;

  /// Defines where to draw port label. The point corresponds to a font baseline.
  Q_INVOKABLE virtual QPointF portTextPosition(NodeId const nodeId, PortType const portType,
                                               PortIndex const portIndex) const = 0;

  /**
   * Defines where to start drawing the caption. The point corresponds to a font
   * baseline.
   */
  Q_INVOKABLE virtual QPointF captionPosition(NodeId const nodeId) const = 0;

  /// Caption rect is needed for estimating the total node size.
  Q_INVOKABLE virtual QRectF captionRect(NodeId const nodeId) const = 0;

  /**
   * Defines where to start drawing the label. The point corresponds to a font
   * baseline.
   */
  Q_INVOKABLE virtual QPointF labelPosition(NodeId const nodeId) const = 0;

  /// Caption rect is needed for estimating the total node size.
  Q_INVOKABLE virtual QRectF labelRect(NodeId const nodeId) const = 0;

  /// Position for an embedded widget. Return any value if you don't embed.
  Q_INVOKABLE virtual QPointF widgetPosition(NodeId const nodeId) const = 0;

  Q_INVOKABLE virtual PortIndex checkPortHit(NodeId const nodeId, PortType const portType,
                                             QPointF const nodePoint) const;

  Q_INVOKABLE virtual QRect resizeHandleRect(NodeId const nodeId) const = 0;

  Q_INVOKABLE virtual int getPortSpacing() = 0;

  protected:
  AbstractGraphModel &_graphModel;
};

Q_DECLARE_INTERFACE(AbstractNodeGeometry, "AbstractNodeGeometry")