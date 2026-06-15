#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <any>
#include <functional>
#include <qlogging.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qvariant.h>
#include <string>

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
  T repr() const {
    auto it = _map.find(QMetaType::fromType<T>());
    if (it == _map.end()) {
      std::string s;
      s.append("Couldn't find type: ");
      s.append(QMetaType::fromType<T>().name());
      throw s.c_str();
    }
    auto fn = *it;
    return std::any_cast<T>(fn());
  }

  /// Type for inner use
  virtual const NodeDataType &type() const = 0;

  protected:
  template <typename T>
  void registerConvert(std::function<T(void)> fn) {
    _map.insert(QMetaType::fromType<T>(), [fn]() { return std::any(fn()); });
  }

  typedef std::function<std::any(void)> ConvertFn;

  QHash<QMetaType, ConvertFn> _map;
};

Q_DECLARE_INTERFACE(NodeData, "NodeData")
