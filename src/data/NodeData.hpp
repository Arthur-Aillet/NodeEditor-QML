#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtQmlIntegration>

#include <any>
#include <functional>
#include <string>

namespace NodeEditor {
/**
 * `id` represents an internal unique data type for the given port.
 * `name` is a normal text description.
 */
struct NodeDataType {
  Q_GADGET
  QML_VALUE_TYPE(nodeDataType)
  QML_STRUCTURED_VALUE

  public:
  using DataTypeId = QString;

  NodeDataType() {}
  NodeDataType(DataTypeId _id, QString _name);
  /// By default, each type is only compatible with itself
  NodeDataType(DataTypeId _id, QString _name, std::vector<DataTypeId> _compatibleTypes);

  DataTypeId id;
  QString name;
  QList<DataTypeId> compatibleTypes;

  Q_PROPERTY(QString id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QList<QString> compatibleTypes MEMBER compatibleTypes)

  bool operator==(const NodeDataType &other) const;
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
  virtual NodeDataType type() const = 0;

  protected:
  template <typename Function>
  void registerConvert(Function fn) {
    _map.insert(QMetaType::fromType<decltype(fn())>(), [fn]() { return std::any(fn()); });
  }

  typedef std::function<std::any(void)> ConvertFn;

  QHash<QMetaType, ConvertFn> _map;
};
}; // namespace NodeEditor

Q_DECLARE_INTERFACE(NodeEditor::NodeData, "NodeData")
