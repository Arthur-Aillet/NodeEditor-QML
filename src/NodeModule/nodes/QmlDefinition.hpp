#pragma once

#include <qobjectdefs.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

namespace NodeRole {
Q_NAMESPACE
QML_ELEMENT

enum class QmlNodeRole {
  Type = 0,              ///< Type of the current node, usually a string.
  Position = 1,          ///< `QPointF` positon of the node on the scene.
  Size = 2,              ///< `QSize` for resizable nodes.
  CaptionVisible = 3,    ///< `bool` for caption visibility.
  Caption = 4,           ///< `QString` for node caption.
  Style = 5,             ///< Custom NodeStyle as QJsonDocument
  InternalData = 6,      ///< Node-stecific user data as QJsonObject
  InPortCount = 7,       ///< `unsigned int`
  OutPortCount = 9,      ///< `unsigned int`
  Widget = 10,           ///< Optional `QWidget*` or `nullptr`
  ValidationState = 11,  ///< Enum NodeValidationState of the node
  LabelVisible = 12,     ///< `bool` for label visibility.
  ProcessingStatus = 13, ///< Enum NodeProcessingStatus of the node
  ProgressValue = 14,    ///< 'QString' for the progress value
  Label = 15,            ///< `QString` for node label.
  LabelEditable = 16,    ///< `bool` to indicate label editing support.
};

Q_ENUM_NS(QmlNodeRole)

} // namespace NodeRole

namespace NodeFlag {
Q_NAMESPACE
QML_ELEMENT

/**
 * Specific flags regulating node features and appeaarence.
 */
enum QmlNodeFlag {
  NoFlags = 0x0,   ///< Default NodeFlag
  Resizable = 0x1, ///< Lets the node be resizable
  Locked = 0x2
};

Q_DECLARE_FLAGS(NodeFlags, QmlNodeFlag)
Q_FLAG_NS(QmlNodeFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(NodeFlags)

} // namespace NodeFlag

namespace PortRole {
Q_NAMESPACE
QML_ELEMENT

/**
 * Constants for fetching port-related information from the GraphModel.
 */
enum class QmlPortRole {
  Data = 0,                 ///< `std::shared_ptr<NodeData>`.
  DataType = 1,             ///< `QString` describing the port data type.
  ConnectionPolicyRole = 2, ///< `enum` ConnectionPolicyRole
  CaptionVisible = 3,       ///< `bool` for caption visibility.
  Caption = 4,              ///< `QString` for port caption.
};
Q_ENUM_NS(QmlPortRole)

} // namespace PortRole

namespace ConnectionPolicy {
Q_NAMESPACE
QML_ELEMENT

/**
 * Defines how many connections are possible to attach to ports. The
 * values are fetched using PortRole::ConnectionPolicy.
 */
enum class QmlConnectionPolicy {
  One,  ///< Just one connection for each port.
  Many, ///< Any number of connections possible for the port.
};
Q_ENUM_NS(QmlConnectionPolicy)

} // namespace ConnectionPolicy

namespace PortType {
Q_NAMESPACE
QML_ELEMENT

/**
 * Used for distinguishing input and output node ports.
 */
enum class QmlPortType {
  In = 0,  ///< Input node port (from the left).
  Out = 1, ///< Output node port (from the right).
  None = 2
};

Q_ENUM_NS(QmlPortType)

} // namespace PortType
