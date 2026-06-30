import QtQuick
import QtQuick.Controls
import CutieDesigner.Stats
import CutieUiModule

PaneBackground {
    clip: true
    Item {
        x: 12
        y: 12
        width: parent.width - 24
        height: parent.height - 24

        Label {
            text: "Stats"
            color: palette.text
        }

        Rectangle {
            y: 20
            color: palette.text
            implicitHeight: 1
            implicitWidth: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
        }

        SimpleFpsCounter {
            y: 36
        }
    }
}
