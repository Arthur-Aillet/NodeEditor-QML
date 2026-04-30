pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeModule

Frame {
    id: root
    visible: true
    clip: true
    padding: 1
    property alias selectedPort: area.selectedPort

    NavigableArea {
        id: area
        width: root.width
        height: root.height

        Repeater {
            delegate: DefaultConnection {
                outPoint: Qt.point(150, 30)
                inPoint: area.mousePosition
                required property real inputId
                connectionId: inputId
            }

            model: ListModel {
                id: connectionModel
            }
        }

        Loader {
            id: temporaryConnection
            active: root.selectedPort !== null
            sourceComponent: DefaultConnection {
                property var type: root.selectedPort["portType"]
                outPoint: type == PortType.In ? area.mousePosition : getPosition()
                inPoint: type == PortType.In ? getPosition() : area.mousePosition
                connectionId: 0

                function getPosition() {
                    const portPos = ModelInterface.nodeGeometry.portPosition(root.selectedPort["nodeId"], type, root.selectedPort["portId"]);
                    const nodePos = ModelInterface.nodeData(root.selectedPort["nodeId"], NodeRole.Position);
                    return Qt.point(portPos.x + nodePos.x, portPos.y + nodePos.y);
                }
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

                Connections {
                    function onPortPicked(portId: real, portType: real) {
                        console.log("picked!");
                        root.selectedPort = {
                            "portId": portId,
                            "nodeId": node.nodeId,
                            "portType": portType
                        };
                    }
                }
            }
            model: ListModel {
                id: nodeModel
            }
        }
    }

    ContextMenu.menu: SceneMenu {
        id: menu
    }

    Connections {
        target: menu
        function onCreateNode(name: string) {
            ModelInterface.createNode(name, area.inner.mapFromItem(root, Qt.point(menu.x, menu.y)));
        }
    }

    Connections {
        target: ModelInterface

        function onConnectionCreated(id: real) {
            connectionModel.append({
                "inputId": id
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
