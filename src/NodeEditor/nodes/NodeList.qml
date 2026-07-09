pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

Item {
    id: nodeList
    required property DraftConnection draftConnection
    required property NavigableArea area

    property alias selectedNodes: selectedNodes

    Clipboard {
        id: clipboard
    }

    function copySelectedNodes() {
        const selectedNodesJson = [];
        const connectionsJson = new Set();
        for (const nodeId of selectedNodes.inner) {
            selectedNodesJson.push(ModelInterface.graph.saveNode(nodeId));
        }
        for (const nodeId of selectedNodes.inner) {
            for (const connection of ModelInterface.graph.allConnectionIds(nodeId)) {
                if (selectedNodes.has(connection.inNodeId) && selectedNodes.has(connection.outNodeId)) {
                    connectionsJson.add(connection);
                }
            }
        }
        clipboard.content = {
            "nodes": selectedNodesJson,
            "connections": Array.from(connectionsJson.values())
        };
    }

    function pasteNodes(): bool {
        const nodeIdMap = new Map();
        let smallestX = Infinity;
        let smallestY = Infinity;
        let largestX = -Infinity;
        let largestY = -Infinity;

        if (!clipboard.content)
            return false;
        const copiedNodes = (clipboard.content as Object)["nodes"];
        if (!copiedNodes || copiedNodes.length == 0)
            return false;
        selectedNodes.clear();
        for (const nodeJson of copiedNodes) {
            const newNodeId = ModelInterface.graph.loadNode(nodeJson);
            nodeIdMap[nodeJson["id"]] = newNodeId;
            const node = nodes.nodeAt(newNodeId);
            if (node.x < smallestX)
                smallestX = node.x;
            if (node.y < smallestY)
                smallestY = node.y;
            if ((node.x + node.width) > largestX)
                largestX = node.x + node.width;
            if ((node.y + node.height) > largestY)
                largestY = node.y + node.height;
        }
        const mediumX = smallestX + ((largestX - smallestX) / 2);
        const mediumY = smallestY + ((largestY - smallestY) / 2);
        for (const oldId in nodeIdMap) {
            const newNodeId = nodeIdMap[oldId];
            const node = nodes.nodeAt(newNodeId);
            const nodePosX = node.x - mediumX + area.mousePosition.x;
            const nodePosY = node.y - mediumY + area.mousePosition.y;
            ModelInterface.graph.setNodeData(newNodeId, NodeEditor.NodeRole.Position, Qt.point(nodePosX, nodePosY));
            selectedNodes.add(newNodeId);
        }
        for (let connection of (clipboard.content as Object)["connections"]) {
            connection.inNodeId = nodeIdMap[connection.inNodeId];
            connection.outNodeId = nodeIdMap[connection.outNodeId];
            ModelInterface.createConnection(connection);
        }
        return true;
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_C && event.modifiers & Qt.ControlModifier) {
            copySelectedNodes();
            event.accepted = true;
        }

        if (event.key == Qt.Key_X && event.modifiers & Qt.ControlModifier) {
            copySelectedNodes();
            for (let id of selectedNodes.inner) {
                event.accepted = ModelInterface.graph.deleteNode(id);
            }
            selectedNodes.clear();
        }

        if (event.key == Qt.Key_V && event.modifiers & Qt.ControlModifier) {
            event.accepted = pasteNodes();
            nodes.nodeAt(selectedNodes.inner[0]).focus = true;
        }

        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            for (let id of selectedNodes.inner) {
                event.accepted = ModelInterface.graph.deleteNode(id);
            }
            selectedNodes.clear();
        }
        if (event.key == Qt.Key_Left) {
            moveSelectedNodes(-5, 0, undefined);
            event.accepted = true;
        }
        if (event.key == Qt.Key_Right) {
            moveSelectedNodes(5, 0, undefined);
            event.accepted = true;
        }
        if (event.key == Qt.Key_Up) {
            moveSelectedNodes(0, -5, undefined);
            event.accepted = true;
        }
        if (event.key == Qt.Key_Down) {
            moveSelectedNodes(0, 5, undefined);
            event.accepted = true;
        }
    }

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

    function moveSelectedNodes(xOffset: int, yOffset: int) {
        for (let nodeId of selectedNodes.inner) {
            let pos = ModelInterface.graph.nodeData(nodeId, NodeEditor.NodeRole.Position);
            pos.x += xOffset;
            pos.y += yOffset;
            ModelInterface.graph.setNodeData(nodeId, NodeEditor.NodeRole.Position, pos);
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
            nodeId: modelId
            nodes: nodeList as NodeList

            draftConnection: nodeList.draftConnection
            area: nodeList.area
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
                "modelId": id
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

            for (let i = 0; i < nodes.count; i++) {
                const current = nodes.itemAt(i) as NodeGraphicalObject;
                if (current.nodeId == id) {
                    if (current.x != position.x)
                        current.x = position.x;
                    if (current.y != position.y)
                        current.y = position.y;
                }
            }
        }
    }
}
