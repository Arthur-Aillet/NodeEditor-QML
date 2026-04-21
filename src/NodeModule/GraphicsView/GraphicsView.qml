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
        NodeObject {}
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
            console.log("newNode!!! " + id);
        }
    }
}
