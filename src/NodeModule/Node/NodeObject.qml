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
    required property var selectedPort
    required property point mousePosition

    onSelectedPortChanged: {
        if (selectedPort == null) {
            focusPolicy = Qt.StrongFocus;
        } else {
            focus = false;
            focusPolicy = Qt.NoFocus;
        }
    }

    property nodeStyle style
    property bool hovered: root.hovered

    Component.onCompleted: {
        const json = ModelInterface.nodeData(nodeId, NodeRole.Style);
        style.loadJson(json);
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            event.accepted = ModelInterface.deleteNode(nodeId);
        }
    }

    onXChanged: {
        ModelInterface.setNodeData(nodeId, NodeRole.Position, Qt.point(x, y));
    }
    onYChanged: {
        ModelInterface.setNodeData(nodeId, NodeRole.Position, Qt.point(x, y));
    }

    drag {
        filterChildren: true
        target: root.focus ? root : undefined
        onActiveChanged: {
            if (drag.active) {
                focus = true;
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
        selectedPort: root.selectedPort
        mousePosition: root.mousePosition
    }

    Repeater {
        id: inOutRepeater
        property int inPortCount: ModelInterface.nodeData(root.nodeId, NodeRole.InPortCount)
        model: inPortCount + ModelInterface.nodeData(root.nodeId, NodeRole.OutPortCount)
        delegate: MouseArea {
            required property int index
            property int portId: index % inOutRepeater.inPortCount
            property var side: (index < inOutRepeater.inPortCount) ? PortType.In : PortType.Out

            property point pos: ModelInterface.nodeGeometry.portPosition(root.nodeId, side, portId)
            property real radius: root.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

            x: pos.x - radius * 1.5
            y: pos.y - radius * 1.5

            width: radius * 2 * 1.5
            height: radius * 2 * 1.5

            onPressed: root.portPicked(portId, side)
        }
    }
}
