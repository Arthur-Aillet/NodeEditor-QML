pragma ComponentBehavior: Bound
import QtQuick
import NodeModule

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
            const node = nodes.itemAt(i) as NodeObject;
            if (node.focus == true) {
                return;
            }
        }
        selectedNodes.clear();
    }

    function moveOtherSelectedNodes(xOffset, yOffset, originId) {
        for (let i = 0; i != nodes.count; i++) {
            const node = nodes.itemAt(i) as NodeObject;
            if (node.selected == true && node.nodeId != originId) {
                node.x += xOffset;
                node.y += yOffset;
            }
        }
    }

    Repeater {
        id: nodes
        delegate: NodeObject {
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
        target: ModelInterface

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
            const position = ModelInterface.nodeData(id, NodeRole.Position);

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
