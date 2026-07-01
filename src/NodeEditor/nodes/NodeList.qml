pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

Item {
    id: root
    required property DraftConnection draftConnection
    required property NavigableArea area

    property alias selectedNodes: selectedNodes

    QSet {
        id: selectedNodes
    }

    property alias nodes: nodes

    function loseFocus() {
        for (let i = 0; i != nodes.count; i++) {
            const node = nodes.itemAt(i) as NodeGraphicalObject;
            if (node.focus == true) {
                return;
            }
        }
        selectedNodes.clear();
    }

    function moveSelectedNodes(xOffset: int, yOffset: int, excludedId: int, snapToGrid: bool) {
        for (let i = 0; i != nodes.count; i++) {
            const node = nodes.itemAt(i) as NodeGraphicalObject;
            if (node.selected == true && node.nodeId != excludedId) {
                if (xOffset != 0) {
                    if (snapToGrid) {
                        node.x = Math.ceil((node.x + xOffset) / 5) * 5;
                    } else {
                        node.x += xOffset;
                    }
                }
                if (yOffset != 0) {
                    if (snapToGrid) {
                        node.y = Math.ceil((node.y + yOffset) / 5) * 5;
                    } else {
                        node.y += yOffset;
                    }
                }
            }
        }
    }

    Repeater {
        id: nodes

        function nodeAt(nodeId: int): var {
            for (let i = 0; i != count; i++) {
                const node = itemAt(i) as NodeGraphicalObject;
                if ((node).nodeId == nodeId)
                    return node;
            }
            return undefined;
        }

        delegate: NodeGraphicalObject {
            id: node
            required property real modelId
            required property real posX
            required property real posY
            nodeId: modelId
            nodes: root as NodeList
            x: posX
            y: posY

            draftConnection: root.draftConnection
            area: root.area
        }
        model: ListModel {
            id: nodeModel
        }
    }

    Connections {
        target: ModelInterface.graph

        function onConnectionCreated(connection: connectionId) {
            nodes.nodeAt(connection.inNodeId).connectionChanged(connection.inPortIndex, NodeEditor.PortType.In, connection.outNodeId, connection.outPortIndex);
            nodes.nodeAt(connection.outNodeId).connectionChanged(connection.outPortIndex, NodeEditor.PortType.Out, connection.inNodeId, connection.inPortIndex);
        }

        function onConnectionDeleted(connection: connectionId) {
            nodes.nodeAt(connection.inNodeId).connectionChanged(connection.inPortIndex, NodeEditor.PortType.In, connection.outNodeId, connection.outPortIndex);
            nodes.nodeAt(connection.outNodeId).connectionChanged(connection.outPortIndex, NodeEditor.PortType.Out, connection.inNodeId, connection.inPortIndex);
        }

        function onNodePortsUpdated(nodeId: int, portType: int) {
            nodes.nodeAt(nodeId).portsChanged(portType);
        }

        function onNodeFlagsUpdated(id: real) {
            for (let i = 0; i < nodes.count; i++) {
                const current = nodes.itemAt(i) as NodeGraphicalObject;
                if (current.nodeId == id)
                    current.loadFlags();
            }
        }

        function onNodeCreated(id: real) {
            nodeModel.append({
                "modelId": id,
                "posX": 0,
                "posY": 0
            });
        }

        function onNodeDeleted(id: real) {
            for (let i = 0; i < nodeModel.count; ++i) {
                if (nodeModel.get(i).modelId == id)
                    nodeModel.remove(i);
            }
        }

        function onNodePositionUpdated(id: real) {
            const position = ModelInterface.graph.nodeData(id, NodeEditor.NodeRole.Position);

            for (let i = 0; i < nodeModel.count; i++) {
                const current = nodeModel.get(i);
                if (current.modelId == id) {
                    if (current.posX != position.x)
                        current.posX = position.x;
                    if (current.posY != position.y)
                        current.posY = position.y;
                }
            }
        }
    }
}
