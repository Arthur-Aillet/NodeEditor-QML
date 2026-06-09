import QtQuick
import CutieDesignerModule

Rectangle {
    required property RectangleNode node

    anchors.fill: parent
    color: node.color
}
