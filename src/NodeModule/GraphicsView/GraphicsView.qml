pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeModule

Frame {
    id: root
    visible: true
    clip: true
    padding: 1

    property var selectedPort: null

    NavigableArea {
        id: area
        width: root.width
        height: root.height
        holdingItem: root.selectedPort !== null

        onDroppedItem: {
            root.selectedPort = null;
        }

        Repeater {
            delegate: DefaultConnection {
                required property real inputInNodeId
                required property real inputInPortIndex
                required property real inputOutNodeId
                required property real inputOutPortIndex
                inNodeId: inputInNodeId
                inPortIndex: inputInPortIndex
                outNodeId: inputOutNodeId
                outPortIndex: inputOutPortIndex

                mousePosition: area.mousePosition
            }

            model: ListModel {
                id: connectionModel
            }
        }

        Loader {
            id: temporaryConnection
            active: root.selectedPort !== null
            sourceComponent: DefaultConnection {
                mousePosition: area.mousePosition

                property bool isPortInput: root.selectedPort.portType == PortType.In

                inNodeId: isPortInput ? root.selectedPort.nodeId : undefined
                inPortIndex: isPortInput ? root.selectedPort.portId : undefined
                outNodeId: isPortInput ? undefined : root.selectedPort.nodeId
                outPortIndex: isPortInput ? undefined : root.selectedPort.portId
            }
        }

        Repeater {
            delegate: NodeObject {
                id: node
                required property real modelId
                required property real posX
                required property real posY
                nodeId: modelId
                x: posX
                y: posY

                selectedPort: root.selectedPort
                mousePosition: area.mousePosition

                onPortPicked: (portId, portType) => {
                    root.selectedPort = {
                        "portId": portId,
                        "nodeId": node.nodeId,
                        "portType": portType
                    };
                }
            }
            model: ListModel {
                id: nodeModel
            }
        }
    }

    ContextMenu.menu: SceneMenu {
        id: menu
        onCreateNode: name => {
            ModelInterface.createNode(name, area.inner.mapFromItem(root, Qt.point(x, y)));
        }
    }

    Connections {
        target: ModelInterface

        function onConnectionCreated(inNodeId, inPortIndex, outNodeId, outPortIndex) {
            connectionModel.append({
                "inputInNodeId": inNodeId,
                "inputInPortIndex": inPortIndex,
                "inputOutNodeId": outNodeId,
                "inputOutPortIndex": outPortIndex
            });
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
