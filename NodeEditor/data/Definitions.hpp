#pragma once

#include <QtCore/QMetaObject>

#include <limits>
#include <qjsvalue.h>
#include <qlist.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qvariant.h>

/**
 * @file
 * Important definitions used throughout the library.
 */

namespace NodeEditor {
Q_NAMESPACE
QML_ELEMENT

/**
 * Constants used for fetching QVariant data from GraphModel.
 */
enum class NodeRole {
  Type = 0,              ///< Type of the current node, usually a string.
  Position = 1,          ///< `QPointF` positon of the node on the scene.
  Size = 2,              ///< `QSize` for resizable nodes.
  CaptionVisible = 3,    ///< `bool` for caption visibility.
  Caption = 4,           ///< `QString` for node caption.
  Style = 5,             ///< Custom NodeStyle as QJsonDocument
  InternalData = 6,      ///< Node-stecific user data as QJsonObject
  InPortCount = 7,       ///< `unsigned int`
  OutPortCount = 9,      ///< `unsigned int`
  ValidationState = 10,  ///< Enum NodeValidationState of the node
  LabelVisible = 11,     ///< `bool` for label visibility.
  ProcessingStatus = 12, ///< Enum NodeProcessingStatus of the node
  ProgressValue = 13,    ///< 'QString' for the progress value
  Label = 14,            ///< `QString` for node label.
  LabelEditable = 15,    ///< `bool` to indicate label editing support.
  Flags = 16,            ///< `NodeFlag` enum value regulating node features
};

Q_ENUM_NS(NodeRole)

/**
 * Specific flags regulating node features and appearence.
 */
enum NodeFlag {
  NoFlags = 0x0,   ///< Default NodeFlag
  Resizable = 0x1, ///< Lets the node be resizable
  Locked = 0x2,    ///< Prevent the no
};

Q_DECLARE_FLAGS(NodeFlags, NodeFlag)
Q_FLAG_NS(NodeFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(NodeFlags)

/**
 * Constants for fetching port-related information from the GraphModel.
 */
enum class PortRole {
  Data = 0,                 ///< `std::shared_ptr<NodeData>`.
  DataType = 1,             ///< `nodeDataType` representing the port data type.
  ConnectionPolicyRole = 2, ///< `enum` ConnectionPolicyRole
  PortCaptionVisible = 3,   ///< `bool` for caption visibility.
  PortCaption = 4,          ///< `QString` for port caption.
};
Q_ENUM_NS(PortRole)

/**
 * Defines how many connections are possible to attach to ports. The
 * values are fetched using PortRole::ConnectionPolicy.
 */
enum class ConnectionPolicy {
  One,     ///< Just one connection for each port.
  Replace, ///< Just one connection for each port. Replace the existing connection if necessary.
  Many,    ///< Any number of connections possible for the port.
};
Q_ENUM_NS(ConnectionPolicy)

/**
 * Used for distinguishing input and output node ports.
 */
enum class PortSide {
  In = 0,  ///< Input node port (from the left).
  Out = 1, ///< Output node port (from the right).
};
Q_ENUM_NS(PortSide)

using PortCount = unsigned int;

/// ports are consecutively numbered starting from zero.
using PortIndex = unsigned int;

/// Unique Id associated with each node in the GraphModel.
using NodeId = unsigned int;

/// Unique Id associated with each node group.
using GroupId = unsigned int;

}; // namespace NodeEditor

using NodeRole = NodeEditor::NodeRole;
using GroupId = NodeEditor::GroupId;
using NodeId = NodeEditor::NodeId;
using PortIndex = NodeEditor::PortIndex;
using PortSide = NodeEditor::PortSide;
using PortCount = NodeEditor::PortCount;
using ConnectionPolicy = NodeEditor::ConnectionPolicy;
using PortRole = NodeEditor::PortRole;
using NodeFlag = NodeEditor::NodeFlag;
using NodeFlags = NodeEditor::NodeFlags;

static constexpr PortIndex InvalidPortIndex = std::numeric_limits<PortIndex>::max();
static constexpr NodeId InvalidNodeId = std::numeric_limits<NodeId>::max();
static constexpr GroupId InvalidGroupId = std::numeric_limits<GroupId>::max();

/// Access some constants from QML
class NodeEditorUtils : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(uint InvalidPortIndex READ invalidPortIndex CONSTANT)
  Q_PROPERTY(uint InvalidNodeId READ invalidNodeId CONSTANT)
  Q_PROPERTY(uint InvalidGroupId READ invalidGroupId CONSTANT)

  inline PortIndex invalidPortIndex() const { return InvalidPortIndex; }
  inline NodeId invalidNodeId() const { return InvalidNodeId; }
  inline GroupId invalidGroupId() const { return InvalidGroupId; }

  public:
  Q_INVOKABLE inline PortSide oppositeSide(PortSide side) {
    if (side == PortSide::In)
      return PortSide::Out;
    return PortSide::In;
  }
};

/**
 * A unique connection identificator that stores
 * out `NodeId`, out `PortIndex`, in `NodeId`, in `PortIndex`
 */
struct ConnectionId {
  Q_GADGET
  QML_VALUE_TYPE(connectionId)
  QML_STRUCTURED_VALUE

  public:
  NodeId outNodeId;
  PortIndex outPortIndex;
  NodeId inNodeId;
  PortIndex inPortIndex;

  Q_PROPERTY(uint outNodeId MEMBER outNodeId)
  Q_PROPERTY(uint outPortIndex MEMBER outPortIndex)
  Q_PROPERTY(uint inNodeId MEMBER inNodeId)
  Q_PROPERTY(uint inPortIndex MEMBER inPortIndex)

  bool operator==(const ConnectionId &other) const {
    return outNodeId == other.outNodeId && outPortIndex == other.outPortIndex &&
           inNodeId == other.inNodeId && inPortIndex == other.inPortIndex;
  }
};

inline void invertConnection(ConnectionId &id) {
  std::swap(id.outNodeId, id.inNodeId);
  std::swap(id.outPortIndex, id.inPortIndex);
}
