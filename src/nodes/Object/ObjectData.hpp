#pragma once

#include "NodeData.hpp"
#include <memory>
#include <qobject.h>
#include <qqmlcomponent.h>
#include <qqmlintegration.h>
#include <qquickitem.h>
#include <qtmetamacros.h>
#include <qvariant.h>

class ObjectData : public NodeData {
  Q_OBJECT
  QML_IMPLEMENTS_INTERFACES(NodeData)
  QML_ELEMENT
  QML_UNCREATABLE("")

  public:
  ObjectData() {}
  ObjectData(std::unique_ptr<QQmlComponent> &&_object, QVariantMap _initialProps = {});

  static inline const NodeDataType dataType = NodeDataType("object", "2D Object");

  const NodeDataType &type() const override { return dataType; }

  std::unique_ptr<QQmlComponent> component;
  QVariantMap initialProps;

  signals:
  void componentLoaded(QQuickItem *object);
};
