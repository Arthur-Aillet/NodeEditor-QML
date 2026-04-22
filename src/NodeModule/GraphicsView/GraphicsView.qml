import QtQuick
import QtQuick.Controls
import NodeModule

Frame {
    id: root
    visible: true
    clip: true
    padding: 1

    NavigableArea {
        width: root.width
        height: root.height
        Repeater {
            delegate: NodeObject {
                required property real newId
                nodeId: newId
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
                "newId": id
            });
            console.log("newNode!!! " + id);
        }
    }
}
