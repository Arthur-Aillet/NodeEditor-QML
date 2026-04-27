import QtQuick
import NodeModule

FocusScope {
    id: root
    opacity: focus ? 1 : style.opacity
    focus: false

    width: painter.width
    height: painter.height

    required property real nodeId
    property nodeStyle style
    property bool hovered: hover.hovered

    DefaultNode {
        id: painter
        nodeId: root.nodeId
        style: root.style
        selected: root.focus
        hovered: root.hovered
    }

    Component.onCompleted: () => {
        var json = ModelInterface.nodeData(nodeId, NodeRole.Style);
        style.loadJson(json);
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            event.accepted = ModelInterface.deleteNode(nodeId);
        }
    }

    MouseArea {
        propagateComposedEvents: true
        anchors.fill: root
        onClicked: {
            parent.focus = !parent.focus;
        }

        HoverHandler {
            id: hover
            grabPermissions: PointerHandler.CanTakeOverFromAnything
            acceptedDevices: PointerDevice.AllPointerTypes
            cursorShape: Qt.CrossCursor
        }
    }
}
