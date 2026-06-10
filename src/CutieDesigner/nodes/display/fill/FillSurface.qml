import QtQuick
import CutieDesigner

Rectangle {
    required property FillNode node

    anchors.fill: parent
    color: node.color
}
