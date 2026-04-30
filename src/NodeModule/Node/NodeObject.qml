import QtQuick
import NodeModule

MouseArea {
    id: root
    opacity: focus ? 1 : style.opacity
    focusPolicy: Qt.StrongFocus

    width: painter.width
    height: painter.height

    signal portPicked(portId: real, portType: real)

    required property real nodeId
    property nodeStyle style
    property bool hovered: root.hovered

    Component.onCompleted: () => {
        const json = ModelInterface.nodeData(nodeId, NodeRole.Style);
        style.loadJson(json);
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            event.accepted = ModelInterface.deleteNode(nodeId);
        }
    }

    drag {
        filterChildren: true
        target: root.focus ? root : undefined
        onActiveChanged: {
            if (drag.active) {
                focus = true;
            }

            if (!drag.active) {
                ModelInterface.setNodeData(nodeId, NodeRole.Position, Qt.point(x, y));
            }
        }
    }
    propagateComposedEvents: true
    anchors.fill: root
    cursorShape: Qt.DragMoveCursor

    DefaultNode {
        id: painter
        nodeId: root.nodeId
        style: root.style
        selected: root.focus
        hovered: root.hovered
        Connections {
            function onPortPicked(portId: real, portType: real) {
                root.portPicked(portId, portType);
            }
        }
    }
}
