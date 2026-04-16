import QtQuick

Item {
    id: root
    anchors.fill: parent
    visible: true
    NodeEditor {
        id: qmlwrap
        width: root.width
        height: root.height
    }
}
