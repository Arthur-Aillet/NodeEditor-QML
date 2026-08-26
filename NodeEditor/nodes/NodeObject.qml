pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

MouseArea {
    id: nodeObj
    opacity: selected ? 1 : style.opacity
    focusPolicy: Qt.StrongFocus

    signal connectionChanged(portIndex: int, portSide: int, otherNodeId: int, otherPortId: int)
    signal portsChanged(side: int)

    required property DraftConnection draftConnection
    required property NavigableArea area
    required property AbstractContext context
    required property NodeList nodes
    required property int nodeId

    property int inPortCount: context.graphModel.nodeData(nodeId, NodeEditor.NodeRole.InPortCount)
    property int outPortCount: context.graphModel.nodeData(nodeId, NodeEditor.NodeRole.OutPortCount)

    property var getPortPosition: (painter.item as AbstractNodePainter).getPortPosition

    property nodeStyle style
    property int flags
    property bool locked: flags & NodeEditor.NodeFlags.Locked

    Component.onCompleted: {
        const newStyle = context.graphModel.nodeData(nodeId, NodeEditor.NodeRole.Style);
        if (newStyle) {
            style = newStyle;
        } else {
            style = context.styles.node;
        }
        loadFlags();
    }

    Connections {
        target: nodeObj
        function onPortsChanged(side: int) {
            if (side == NodeEditor.PortSide.In) {
                nodeObj.inPortCount = nodeObj.context.graphModel.nodeData(nodeObj.nodeId, NodeEditor.NodeRole.InPortCount);
            } else if (side == NodeEditor.PortSide.Out) {
                nodeObj.outPortCount = nodeObj.context.graphModel.nodeData(nodeObj.nodeId, NodeEditor.NodeRole.OutPortCount);
            }
        }
    }

    Connections {
        target: nodeObj.nodes
        function onSelectedNodesSizeChanged() {
            nodeObj.selected = nodeObj.nodes.selectedNodes.has(nodeObj.nodeId);
        }
    }

    function loadFlags() {
        flags = context.graphModel.nodeData(nodeId, NodeEditor.NodeRole.Flags);
    }

    property bool selected: false

    property bool waitForClick: false
    property bool waitForFocus: false

    onFocusChanged: {
        if (!focus) {
            nodes.loseFocus();
        }
        if (waitForFocus) {
            nodes.selectedNodes.clear();
            nodes.selectedNodes.add(nodeId);
            nodes.selectedNodesSize = nodes.selectedNodes.size;
        }
        waitForFocus = false;
    }

    onClicked: mouse => {
        if (mouse.modifiers & Qt.ShiftModifier) {
            if (selected && !waitForClick) {
                nodes.selectedNodes.delete(nodeId);
                nodes.selectedNodesSize = nodes.selectedNodes.size;
            }
        }
        waitForClick = false;
    }

    onPressed: mouse => {
        if (locked)
            return;

        if (mouse.modifiers & Qt.ShiftModifier) {
            if (!selected) {
                nodes.selectedNodes.add(nodeId);
                nodes.selectedNodesSize = nodes.selectedNodes.size;
                waitForClick = true;
            }
        } else {
            if (focus) {
                nodes.selectedNodes.clear();
                nodes.selectedNodes.add(nodeId);
                nodes.selectedNodesSize = nodes.selectedNodes.size;
            } else {
                waitForFocus = true;
            }
        }
    }

    Connections {
        target: nodeObj.draftConnection

        function onSelectedPortChanged() {
            if (nodeObj.draftConnection.selectedPort == null) {
                nodeObj.focusPolicy = Qt.StrongFocus;
            } else {
                nodeObj.nodes.selectedNodes.clear();
                nodeObj.nodes.selectedNodesSize = nodeObj.nodes.selectedNodes.size;
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
            nodes.moveSelectedNodes(x - xPrev, y - yPrev);
            xPrev = x;
            yPrev = y;
        }
    }

    drag {
        filterChildren: true
        target: selected ? nodeObj : undefined
        onActiveChanged: {
            if (drag.active) {
                focus = true;
            }
        }
    }

    hoverEnabled: true
    cursorShape: locked ? Qt.ArrowCursor : Qt.DragMoveCursor

    width: painter.width
    height: painter.height

    Loader {
        id: painter

        Component.onCompleted: {
            setSource(nodeObj.context.nodePainter.url, {
                nodeObject: nodeObj,
                area: nodeObj.area,
                draftConnection: nodeObj.draftConnection,
                context: nodeObj.context
            });
        }
        onItemChanged: nodeObj.context.graphModel.createEmbed(nodeObj.nodeId, (item as AbstractNodePainter).embeddedComponentContainer)
    }

    // Port Interaction points
    Repeater {
        model: nodeObj.inPortCount + nodeObj.outPortCount
        delegate: MouseArea {
            required property int index
            property var side: (index < nodeObj.inPortCount) ? NodeEditor.PortSide.In : NodeEditor.PortSide.Out
            property int portId: (index < nodeObj.inPortCount) ? index : index - nodeObj.inPortCount

            property point pos: (painter.item as AbstractNodePainter).getPortPosition(portId, side)
            property real radius: nodeObj.style.connectionPointDiameter * 1.8 // Diameter is used a the radius in the original

            // Small offset in the direction of the port for easier click
            x: pos.x - radius + (side == NodeEditor.PortSide.In ? -nodeObj.style.connectionPointDiameter : nodeObj.style.connectionPointDiameter)
            y: pos.y - radius

            width: radius * 2
            height: radius * 2

            onPressed: nodeObj.draftConnection.selectedPort = {
                "portId": portId,
                "nodeId": nodeObj.nodeId,
                "portSide": side
            }
        }
    }
}
