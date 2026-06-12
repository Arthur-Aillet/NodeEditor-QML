#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <qdebug.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>
#include <vector>

/**
 * `id` represents an internal unique data type for the given port.
 * `name` is a normal text description.
 */
struct NodeDataType {
  using DataTypeId = QString;
  NodeDataType(DataTypeId _id, QString _name) : id(_id), name(_name) {}
  DataTypeId id;
  QString name;

  // By default, each type is only compatible with itself
  virtual QList<QString> compatibleTypes() const { return QList<DataTypeId>(id); }
};

/**
 * Class represents data transferred between nodes.
 * @param type is used for comparing the types
 * The actual data is stored in subtypes
 */
class NodeData : public QObject {
  Q_OBJECT
  QML_INTERFACE

  public:
  virtual ~NodeData() = default;

  template <typename T>
  const T &repr() const {
    const QVariant &variant = _map.find(QMetaType::fromType<T>()).value();
    return *reinterpret_cast<const T *>(variant.constData());
  }

  /// Type for inner use
  virtual const NodeDataType &type() const = 0;

  protected:
  typedef std::function<void(void)> BindingFn;
  typedef QPropertyChangeHandler<BindingFn> BindingFnHandler;

  QHash<QMetaType, QVariant> _map;
  std::vector<BindingFnHandler> _registeredBindings;
  QVariant err = QVariant::fromMetaType(QMetaType(QMetaType::UnknownType));
};

Q_DECLARE_INTERFACE(NodeData, "NodeData")
