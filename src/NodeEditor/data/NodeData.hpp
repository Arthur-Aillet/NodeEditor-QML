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

  virtual bool sameType(NodeData const &nodeData) const {
    return (this->type().id == nodeData.type().id);
  }

  template <typename T>
  const T &get() const {
    const QVariant &variant = get(QMetaType::fromType<T>());
    return *reinterpret_cast<const T *>(variant.constData());
  }

  /// Type for inner use
  virtual const NodeDataType &type() const = 0;

  protected:
  virtual const QVariant &get(QMetaType type) const { return err; }

  typedef std::function<void(void)> BindingFn;
  typedef QPropertyChangeHandler<BindingFn> BindingFnHandler;

  std::vector<BindingFnHandler> _registeredBindings;
  QVariant err = QVariant::fromMetaType(QMetaType(QMetaType::UnknownType));
};

Q_DECLARE_INTERFACE(NodeData, "NodeData")
