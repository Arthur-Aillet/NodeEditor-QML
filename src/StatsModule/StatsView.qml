import QtQuick
import QtQuick.Controls
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
            id: src
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

    Item {
        x: 12
        y: 12
        width: parent.width - 24
        height: parent.height - 24

        Label {
            text: "Stats"
            color: "white"
        }

        Rectangle {
            y: 20
            color: "lightgrey"
            implicitHeight: 1
            implicitWidth: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
        }

        SimpleFpsCounter {
            y: 36
        }
    }
}
