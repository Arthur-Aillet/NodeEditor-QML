import QtQuick
import CutieDesignerModule

Rectangle {
    required property FillNode node

    anchors.fill: parent
    color: node.color
}
