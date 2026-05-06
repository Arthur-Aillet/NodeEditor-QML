pragma ComponentBehavior: Bound
import QtQuick
import NodeModule

MouseArea {
    id: root
    opacity: selected ? 1 : style.opacity
    focusPolicy: Qt.StrongFocus

    required property DraftConnection draftConnection
    required property NavigableArea area
    required property NodeList nodes
    required property int nodeId
    property bool selected: false

    Connections {
        target: root.nodes.selectedNodes
        function onChanged() {
            root.selected = root.nodes.selectedNodes.has(root.nodeId);
        }
    }

    width: painter.width
    height: painter.height

    onFocusChanged: {
        if (!focus) {
            nodes.loseFocus();
        }
    }

    onClicked: mouse => {
        if (mouse.modifiers & Qt.ShiftModifier) {
            if (selected && !waitForClick) {
                nodes.selectedNodes.remove(nodeId);
            }
        }
        waitForClick = false;
    }

    property bool waitForClick: false

    onPressed: mouse => {
        if (mouse.modifiers & Qt.ShiftModifier) {
            if (!selected) {
                nodes.selectedNodes.add(nodeId);
                waitForClick = true;
            }
        } else {
            nodes.selectedNodes.clear();
            nodes.selectedNodes.add(nodeId);
        }
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            for (let id of nodes.selectedNodes.inner) {
                event.accepted = ModelInterface.deleteNode(id);
            }
        }
    }

    Connections {
        target: root.draftConnection

        function onSelectedPortChanged() {
            if (root.draftConnection.selectedPort == null) {
                root.focusPolicy = Qt.StrongFocus;
            } else {
                root.focus = false;
                root.focusPolicy = Qt.NoFocus;
            }
        }
    }

    property nodeStyle style
    property bool hovered: root.hovered

    Component.onCompleted: {
        const json = ModelInterface.nodeData(nodeId, NodeRole.Style);
        style.loadJson(json);
    }

    property real xPrev
    property real yPrev

    onXChanged: {
        if (!drag.active)
            xPrev = x;
    }

    onYChanged: {
        if (!drag.active)
            yPrev = y;
    }

    onPositionChanged: mouse => {
        if (drag.active) {
            nodes.moveOtherSelectedNodes(x - xPrev, y - yPrev, nodeId);
            xPrev = x;
            yPrev = y;
        }
    }

    drag {
        filterChildren: true
        target: root.selected ? root : undefined
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
        selected: root.selected
        hovered: root.hovered
        selectedPort: root.draftConnection.selectedPort
        mousePosition: root.area.mousePosition
        nodePosition: Qt.point(root.x, root.y)
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

            onPressed: {
                root.draftConnection.selectedPort = {
                    "portId": portId,
                    "nodeId": root.nodeId,
                    "portType": side
                };
            }
        }
    }
}
