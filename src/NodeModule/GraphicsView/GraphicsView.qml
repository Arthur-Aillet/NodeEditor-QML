pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeModule

Frame {
    id: root
    visible: true
    clip: true
    padding: 1

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
        Repeater {
            delegate: NodeObject {
                required property real inputId
                required property real inputX
                required property real inputY
                nodeId: inputId
                x: inputX
                y: inputY
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
