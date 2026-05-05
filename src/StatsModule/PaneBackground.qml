import QtQuick
import NodeModule
import "InnerShadow"

Rectangle {
    id: root
    color: Qt.darker(StyleCollection.graphicsViewStyle.backgroundColor, 2)

    Item {
        id: background
        anchors.fill: parent

        layer.enabled: true
        layer.smooth: true
        visible: false

        Rectangle {
            id: inner
            width: parent.width - 4
            height: parent.height - 4
            x: 2
            y: 2
            color: StyleCollection.graphicsViewStyle.backgroundColor
        }
    }

    InnerShadow {
        anchors.fill: background
        source: background
        innerShadowColor: root.color
    }
}
