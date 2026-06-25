import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CutieUiModule
import CutieDesigner.Time

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
            id: slider
            direction: FlexboxLayout.RowReverse
            min: TimeController.minPos
            max: TimeController.maxPos
            value: {
                value = TimeController.currentPos;
            }
            onValueChanged: {
                TimeController.currentPos = value;
            }
            Connections {
                target: TimeController
                function onCurrentPosChanged() {
                    slider.value = TimeController.currentPos;
                }
            }
            Layout.fillWidth: true
        }

        PlayPauseButton {
            playing: TimeController.playing
            onClicked: TimeController.playing = !TimeController.playing
        }
        MediaButton {
            type: MediaButton.MediaButtonType.Stop
            onClicked: TimeController.stop()
        }

        Text {
            text: "Start: "
            color: "white"
        }
        TextField {
            value: {
                value = TimeController.minPos;
            }
            textField.placeholderText: "sec"
            onTextChanged: {
                TimeController.minPos = value;
                slider.inner.value = (TimeController.currentPos - TimeController.minPos) / TimeController.maxPos;
            }
        }
        Text {
            text: "End: "
            color: "white"
        }
        TextField {
            value: {
                value = TimeController.maxPos;
            }
            textField.placeholderText: "sec"
            onTextChanged: {
                TimeController.maxPos = value;
                slider.inner.value = (TimeController.currentPos - TimeController.minPos) / TimeController.maxPos;
            }
        }
    }
}
