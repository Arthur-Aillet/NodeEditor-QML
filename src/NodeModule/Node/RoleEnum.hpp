#pragma once

#include <qobjectdefs.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

namespace NodeRole {
Q_NAMESPACE
QML_ELEMENT

enum class NodeRoles {
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

Q_ENUM_NS(NodeRoles)

} // namespace NodeRole
