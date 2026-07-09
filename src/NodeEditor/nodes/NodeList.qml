pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

Item {
    id: nodeList
    required property DraftConnection draftConnection
    required property NavigableArea area

    property alias selectedNodes: selectedNodes
    property int counter: 0
    Clipboard {
        id: clipboard
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_C) {
            clipboard.text = counter;
            counter++;
        }
        if (event.key == Qt.Key_V) {
            console.log(clipboard.text);
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
