pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

MouseArea {
    id: nodeObj
    opacity: selected ? 1 : style.opacity
    focusPolicy: Qt.StrongFocus

    signal connectionChanged(portIndex: int, portType: int, otherNodeId: int, otherPortId: int)
    signal portsChanged(type: int)

    required property DraftConnection draftConnection
    required property NavigableArea area
    required property NodeList nodes
    required property int nodeId

    property int inPortCount: ModelInterface.graph.nodeData(nodeObj.nodeId, NodeEditor.NodeRole.InPortCount)
    property int outPortCount: ModelInterface.graph.nodeData(nodeObj.nodeId, NodeEditor.NodeRole.OutPortCount)

    property var getPortPosition: (painter.item as AbstractNodePainter).getPortPosition

    property nodeStyle style
    property int flags
    property bool locked: flags & NodeEditor.NodeFlags.Locked

    Connections {
        target: nodeObj
        function onPortsChanged(type: int) {
            if (type == NodeEditor.PortType.In) {
                nodeObj.inPortCount = ModelInterface.graph.nodeData(nodeObj.nodeId, NodeEditor.NodeRole.InPortCount);
            } else if (type == NodeEditor.PortType.Out) {
                nodeObj.outPortCount = ModelInterface.graph.nodeData(nodeObj.nodeId, NodeEditor.NodeRole.OutPortCount);
            }
        }
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
        target: nodeObj.nodes.selectedNodes
        function onChanged() {
            nodeObj.selected = nodeObj.nodes.selectedNodes.has(nodeObj.nodeId);
        }
    }

    onFocusChanged: {
        if (!focus)
            nodes.loseFocus();
    }

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
        target: nodeObj.draftConnection

        function onSelectedPortChanged() {
            if (nodeObj.draftConnection.selectedPort == null) {
                nodeObj.focusPolicy = Qt.StrongFocus;
            } else {
                nodeObj.nodes.selectedNodes.clear();
                nodeObj.focus = false;
                nodeObj.focusPolicy = Qt.NoFocus;
            }
        }
    }

    // Store movement offset for each frame to apply it to the other selected nodes
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
        target: nodeObj.selected ? nodeObj : undefined
        onActiveChanged: {
            if (drag.active) {
                focus = true;
            }
        }
    }
    propagateComposedEvents: true
    anchors.fill: nodeObj
    hoverEnabled: true
    cursorShape: locked ? Qt.ArrowCursor : Qt.DragMoveCursor

    width: painter.width
    height: painter.height

    Loader {
        id: painter

        property url nodePainterUrl

        onNodePainterUrlChanged: {
            const initialProperties = {
                nodeObject: nodeObj,
                area: nodeObj.area,
                draftConnection: nodeObj.draftConnection
            };
            setSource(nodePainterUrl, initialProperties);
        }
        Component.onCompleted: {
            nodePainterUrl = "DefaultNodePainter.qml";
        }
        onItemChanged: DataFlowModelInterface.dataFlowGraph.requestComponent(nodeObj.nodeId, (item as AbstractNodePainter).embeddedComponentContainer)
    }

    // Port Interaction points
    Repeater {
        model: nodeObj.inPortCount + nodeObj.outPortCount
        delegate: MouseArea {
            required property int index
            property var side: (index < nodeObj.inPortCount) ? NodeEditor.PortType.In : NodeEditor.PortType.Out
            property int portId: (index < nodeObj.inPortCount) ? index : index - nodeObj.inPortCount

            property point pos: (painter.item as AbstractNodePainter).getPortPosition(portId, side)
            property real radius: nodeObj.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

            x: pos.x - radius * 1.5
            y: pos.y - radius * 1.5

            width: radius * 2 * 1.5
            height: radius * 2 * 1.5

            onPressed: nodeObj.draftConnection.selectedPort = {
                "portId": portId,
                "nodeId": nodeObj.nodeId,
                "portType": side
            }
        }
    }
}
