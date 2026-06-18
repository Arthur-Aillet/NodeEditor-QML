#pragma once

#include "NodeData.hpp"
#include <memory>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qqmlintegration.h>
#include <qquickitem.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class SurfaceData : public NodeData {
  Q_OBJECT
  QML_IMPLEMENTS_INTERFACES(NodeData)
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  SurfaceData() {}
  SurfaceData(std::unique_ptr<QQmlComponent> &&object, QVariantMap initialProps = {});

  static inline const NodeDataType dataType = NodeDataType("object", "2D Surface");

  const NodeDataType &type() const override { return dataType; }

  const std::unique_ptr<QQmlComponent> &component() { return _component; };
  const QVariantMap &initialProps() { return _initialProps; };

  protected:
  std::unique_ptr<QQmlComponent> _component;
  QVariantMap _initialProps;

  signals:
  void componentLoaded(QQuickItem *object);
  void componentDestroyed(QObject *object);
};
