import QtQuick
import NodeModule

Item {
    id: root
    opacity: focus ? 1 : style.opacity
    focus: false

    width: painter.width
    height: painter.height

    required property real nodeId
    property nodeStyle style
    property bool hovered: hover.hovered

    Component.onCompleted: () => {
        const json = ModelInterface.nodeData(nodeId, NodeRole.Style);
        style.loadJson(json);
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            event.accepted = ModelInterface.deleteNode(nodeId);
        }
    }

    MouseArea {
        drag {
            filterChildren: true
            target: parent.focus ? parent : undefined
            onActiveChanged: {
                if (drag.active) {
                    parent.focus = true;
                }

                if (!drag.active) {
                    ModelInterface.setNodeData(root.nodeId, NodeRole.Position, Qt.point(parent.x, parent.y))
                }
            }
        }
        propagateComposedEvents: true
        anchors.fill: root
        onClicked: {
            parent.focus = !parent.focus;
        }

        DefaultNode {
            id: painter
            nodeId: root.nodeId
            style: root.style
            selected: root.focus
            hovered: root.hovered
        }

        HoverHandler {
            id: hover
            cursorShape: Qt.CrossCursor
        }
    }
}
