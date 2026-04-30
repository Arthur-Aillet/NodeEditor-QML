pragma ComponentBehavior: Bound
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
    }

    Repeater {
        id: inOutRepeater
        model: 2
        delegate: Repeater {
            id: side
            required property int index

            property var type: (side.index == 0) ? PortType.In : PortType.Out
            model: ModelInterface.nodeData(root.nodeId, (side.index == 0) ? NodeRole.InPortCount : NodeRole.OutPortCount)
            delegate: Item {
                id: port
                required property int index

                property point pos: ModelInterface.nodeGeometry.portPosition(root.nodeId, side.type, port.index)
                property real radius: root.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

                x: pos.x
                y: pos.y

                MouseArea {
                    property real radius: root.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

                    x: -radius * 1.5
                    y: -radius * 1.5
                    width: radius * 2 * 1.5
                    height: radius * 2 * 1.5

                    onPressed: root.portPicked(port.index, side.type)
                }
            }
        }
    }
}
