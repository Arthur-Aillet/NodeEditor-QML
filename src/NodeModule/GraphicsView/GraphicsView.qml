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
                required property real givenId
                nodeId: givenId
            }
            model: ListModel {
                id: model
            }
        }
    }

    ContextMenu.menu: Menu {
        popupType: Popup.Window
        MenuItem {
            text: qsTr("A")
        }
        MenuItem {
            text: qsTr("B")
        }
        MenuItem {
            text: qsTr("C")
        }
    }

    Connections {
        target: ModelInterface
        function onNodeCreated(id: real) {
            model.append({
                "givenId": id
            });
        }
        function onNodeDeleted(id: real) {
            for (let i = 0; i < model.count; ++i) {
                if (model.get(i).givenId == id)
                    model.remove(i);
            }
        }
    }
}
