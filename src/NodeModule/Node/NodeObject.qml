import QtQuick
import NodeModule

Item {
    id: root
    opacity: selected ? 1 : style.opacity

    width: painter.width
    height: painter.height

    required property real nodeId
    property nodeStyle style
    property bool selected: false
    property bool hovered: hover.hovered

    DefaultNode {
        id: painter
        nodeId: root.nodeId
        style: root.style
        selected: root.selected
        hovered: root.hovered
    }

    Component.onCompleted: () => {
        var json = ModelInterface.nodeData(nodeId, NodeRole.Style);
        style.loadJson(json);
    }

    MouseArea {
        propagateComposedEvents: true
        anchors.fill: painter
        onClicked: {
            parent.selected = !parent.selected;
        }

        HoverHandler {
            id: hover
            grabPermissions: PointerHandler.CanTakeOverFromAnything
            acceptedDevices: PointerDevice.AllPointerTypes
            cursorShape: Qt.CrossCursor
        }
    }
}
