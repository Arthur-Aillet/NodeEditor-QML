#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <qdebug.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

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
class NodeData {
  public:
  virtual ~NodeData() = default;

  virtual bool sameType(NodeData const &nodeData) const {
    return (this->type().id == nodeData.type().id);
  }

  /// Type for inner use
  virtual const NodeDataType &type() const = 0;
};

Q_DECLARE_METATYPE(NodeDataType)
Q_DECLARE_METATYPE(std::shared_ptr<NodeData>)
