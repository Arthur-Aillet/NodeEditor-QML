import QtQuick
import CutieDesigner.Nodes

Rectangle {
    required property FillNode node

    anchors.fill: parent
    color: node.color
}
