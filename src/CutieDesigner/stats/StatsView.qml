import QtQuick
import QtQuick.Controls
import CutieDesigner.Stats

PaneBackground {
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
