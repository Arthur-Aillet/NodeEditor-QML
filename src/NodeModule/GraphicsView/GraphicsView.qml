import QtQuick
import QtQuick.Controls
import NodeModule

Frame {
    id: root
    visible: true
    clip: true
    padding: 1
    focus: true

    NavigableArea {
        width: root.width
        height: root.height
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
                id: model
            }
        }
    }

    ContextMenu.menu: SceneMenu {}

    Connections {
        target: ModelInterface
        function onNodeCreated(id: real) {
            model.append({
                "inputId": id,
                "inputX": 0,
                "inputY": 0,
            });
        }

        function onNodeDeleted(id: real) {
            for (let i = 0; i < model.count; ++i) {
                if (model.get(i).inputId == id)
                    model.remove(i);
            }
        }

        function onNodePositionUpdated(id: real) {
            const position = ModelInterface.nodeData(id, NodeRole.Position);

            for (let i = 0; i < model.count; i++) {
                const current = model.get(i);
                if (current.inputId == id) {
                    current.inputX = position.x;
                    current.inputY = position.y;
                }
            }
        }
    }
}
