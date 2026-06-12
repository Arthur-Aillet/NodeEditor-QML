#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <memory>
#include <qdebug.h>
#include <qlogging.h>
#include <qproperty.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>

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
    auto it = _map.find(QMetaType::fromType<T>());
    if (it == _map.end())
      qCritical() << "Couldn't find type" << QMetaType::fromType<T>().name();
    const QVariant &variant = it.value();
    return *reinterpret_cast<const T *>(variant.constData());
  }

  /// Type for inner use
  virtual const NodeDataType &type() const = 0;

  protected:
  typedef std::function<void(void)> BindingFn;
  typedef QPropertyChangeHandler<BindingFn> BindingFnHandler;

  QHash<QMetaType, QVariant> _map;

  void defineBinding(BindingFnHandler &&binding) {
    _registeredBinding = std::make_unique<BindingFnHandler>(std::move(binding));
  }

  private:
  std::unique_ptr<BindingFnHandler> _registeredBinding;
  QVariant err = QVariant::fromMetaType(QMetaType(QMetaType::UnknownType));
};

Q_DECLARE_INTERFACE(NodeData, "NodeData")
