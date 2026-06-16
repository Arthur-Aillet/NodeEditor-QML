pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

MouseArea {
    id: root
    opacity: selected ? 1 : style.opacity
    focusPolicy: Qt.StrongFocus

    signal connectionChanged(portIndex: int, portType: int, otherNodeId: int, otherPortId: int)

    required property DraftConnection draftConnection
    required property NavigableArea area
    required property NodeList nodes
    required property int nodeId

    property int inPortCount: ModelInterface.graph.nodeData(root.nodeId, NodeEditor.NodeRole.InPortCount)
    property int outPortCount: ModelInterface.graph.nodeData(root.nodeId, NodeEditor.NodeRole.OutPortCount)

    property var getPortPosition: (painter.item as AbstractNodePainter).getPortPosition

    property nodeStyle style
    property int flags
    property bool locked: flags & NodeEditor.NodeFlags.Locked

    function portsChanged(type: int) {
        if (type == NodeEditor.PortType.In) {
            inPortCount = ModelInterface.graph.nodeData(root.nodeId, NodeEditor.NodeRole.InPortCount);
        }
        if (type == NodeEditor.PortType.Out)
            outPortCount = ModelInterface.graph.nodeData(root.nodeId, NodeEditor.NodeRole.OutPortCount);
    }

    function loadFlags() {
        flags = ModelInterface.graph.nodeData(nodeId, NodeEditor.NodeRole.Flags);
    }

    Component.onCompleted: {
        const json = ModelInterface.graph.nodeData(nodeId, NodeEditor.NodeRole.Style);
        style.loadJson(json);
        style = style;
        loadFlags();
    }

    property bool selected: false

    Connections {
        target: root.nodes.selectedNodes
        function onChanged() {
            root.selected = root.nodes.selectedNodes.has(root.nodeId);
        }
    }

    onFocusChanged: if (!focus)
        nodes.loseFocus()

    onClicked: mouse => {
        if (mouse.modifiers & Qt.ShiftModifier)
            if (selected && !waitForClick)
                nodes.selectedNodes.remove(nodeId);
        waitForClick = false;
    }

    property bool waitForClick: false

    onPressed: mouse => {
        if (locked)
            return;

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
        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back)
            for (let id of nodes.selectedNodes.inner)
                event.accepted = ModelInterface.graph.deleteNode(id);
    }

    Connections {
        target: root.draftConnection

        function onSelectedPortChanged() {
            if (root.draftConnection.selectedPort == null) {
                root.focusPolicy = Qt.StrongFocus;
            } else {
                root.nodes.selectedNodes.clear();
                root.focus = false;
                root.focusPolicy = Qt.NoFocus;
            }
        }
    }

    // Store movement offset for each frame to apply it to the other selected nodes
    property real xPrev
    property real yPrev

    onXChanged: if (!drag.active)
        xPrev = x
    onYChanged: if (!drag.active)
        yPrev = y

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
        onActiveChanged: if (drag.active)
            focus = true
    }
    propagateComposedEvents: true
    anchors.fill: root
    hoverEnabled: true
    cursorShape: locked ? Qt.ArrowCursor : Qt.DragMoveCursor

    width: painter.width
    height: painter.height

    Loader {
        id: painter

        property url nodePainterUrl

        onNodePainterUrlChanged: {
            const initialProperties = {
                nodeObject: root,
                area: root.area,
                draftConnection: root.draftConnection
            };
            setSource(nodePainterUrl, initialProperties);
        }
        Component.onCompleted: {
            nodePainterUrl = "DefaultNodePainter.qml";
        }
        onItemChanged: DataFlowModelInterface.dataFlowGraph.requestComponent(root.nodeId, (item as AbstractNodePainter).embeddedComponentContainer)
    }

    // Port Interaction points
    Repeater {
        model: root.inPortCount + root.outPortCount
        delegate: MouseArea {
            required property int index
            property var side: (index < root.inPortCount) ? NodeEditor.PortType.In : NodeEditor.PortType.Out
            property int portId: (index < root.inPortCount) ? index : index - root.inPortCount

            property point pos: (painter.item as AbstractNodePainter).getPortPosition(portId, side)
            property real radius: root.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

            x: pos.x - radius * 1.5
            y: pos.y - radius * 1.5

            width: radius * 2 * 1.5
            height: radius * 2 * 1.5

            onPressed: root.draftConnection.selectedPort = {
                "portId": portId,
                "nodeId": root.nodeId,
                "portType": side
            }
        }
    }
}
