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

                inNodeId: root.selectedPort["portType"] == PortType.In ? root.selectedPort["nodeId"] : undefined
                inPortIndex: root.selectedPort["portType"] == PortType.In ? root.selectedPort["portId"] : undefined
                outNodeId: root.selectedPort["portType"] == PortType.In ? undefined : root.selectedPort["nodeId"]
                outPortIndex: root.selectedPort["portType"] == PortType.In ? undefined : root.selectedPort["portId"]
            }
        }

        Repeater {
            delegate: NodeObject {
                id: node
                required property real inputId
                required property real inputX
                required property real inputY
                nodeId: inputId
                x: inputX
                y: inputY
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
                "inputId": id,
                "inputX": 0,
                "inputY": 0
            });
        }

        function onNodeDeleted(id: real) {
            for (let i = 0; i < nodeModel.count; ++i) {
                if (nodeModel.get(i).inputId == id)
                    nodeModel.remove(i);
            }
        }

        function onNodePositionUpdated(id: real) {
            const position = ModelInterface.nodeData(id, NodeRole.Position);

            for (let i = 0; i < nodeModel.count; i++) {
                const current = nodeModel.get(i);
                if (current.inputId == id) {
                    if (current.inputX != position.x)
                        current.inputX = position.x;
                    if (current.inputY != position.y)
                        current.inputY = position.y;
                }
            }
        }
    }
}
