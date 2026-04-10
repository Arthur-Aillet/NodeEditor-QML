import QtQuick

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Sandbox!")
    Rectangle {
        color: '#ff0000'
        width: root.width
        height: root.height
    }
}
