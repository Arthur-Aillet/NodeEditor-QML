import QtQuick
import NodeModule

Rectangle {
    id: root
    opacity: selected ? 1 : 0.7

    property nodeStyle style
    property bool selected

    DefaultNode {
        id: painter
        style: root.style
        selected: root.selected
    }

    MouseArea {
        anchors.fill: painter
        onClicked: {
            parent.selected = !parent.selected;
        }
    }
}
