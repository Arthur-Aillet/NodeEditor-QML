import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CutieUiModule

PaneBackground {
    height: 40

    FlexboxLayout {
        x: 6
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width - 12
        alignItems: FlexboxLayout.AlignCenter
        gap: 6

        SliderField {
            Layout.fillWidth: true
        }

        PlayPauseButton {
            playing: false
            onClicked: playing = !playing
        }
        MediaButton {
            type: MediaButton.MediaButtonType.Stop
        }

        Text {
            text: "Start: "
            color: "white"
        }
        TextField {
            textField.placeholderText: "sec"
        }
        Text {
            text: "End: "
            color: "white"
        }
        TextField {
            textField.placeholderText: "sec"
        }
    }
}
