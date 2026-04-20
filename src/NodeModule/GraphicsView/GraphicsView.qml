import QtQuick
import NodeModule

Item {
    id: root
    visible: true
    NavigableArea {
        width: root.width
        height: root.height
        Rectangle {
            color: "blue"
            width: 100
            height: 100
            x: 30
            y: 50
        }
    }
}
