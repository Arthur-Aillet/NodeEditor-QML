#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <qqmlintegration.h>

/**
 * `id` represents an internal unique data type for the given port.
 * `name` is a normal text description.
 */
struct NodeDataType {
  Q_GADGET
  public:
  QString id;
  QString name;
  Q_PROPERTY(QString id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
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
  virtual NodeDataType type() const = 0;
};

Q_DECLARE_METATYPE(NodeDataType)
Q_DECLARE_METATYPE(std::shared_ptr<NodeData>)
