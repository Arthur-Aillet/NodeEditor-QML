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

        PlayPauseButton {
            playing: TimeController.playing
            onClicked: TimeController.playing = !TimeController.playing
        }

        MediaButton {
            type: MediaButton.MediaButtonType.Stop
            onClicked: TimeController.stop()
        }

        SliderField {
            id: slider
            direction: FlexboxLayout.RowReverse
            min: TimeController.minPos
            max: TimeController.maxPos

            onValueChanged: {
                TimeController.currentPos = value;
            }
            inner.onMoved: {
                TimeController.playing = false;
            }
            Connections {
                target: TimeController
                function onCurrentPosChanged() {
                    slider.value = TimeController.currentPos;
                }
            }
            Layout.fillWidth: true
        }

        Text {
            text: "Start: "
            color: "white"
        }
        TextField {
            value: {
                value = Number(TimeController.minPos).toFixed(2);
            }
            textField.placeholderText: "sec"
            textField.inputMethodHints: Qt.ImhFormattedNumbersOnly
            onTextChanged: {
                TimeController.minPos = value;
                slider.inner.value = (TimeController.currentPos - TimeController.minPos) / (TimeController.maxPos - TimeController.minPos);
            }
        }
        Text {
            text: "End: "
            color: "white"
        }
        TextField {
            value: {
                value = Number(TimeController.maxPos).toFixed(2);
            }
            textField.placeholderText: "sec"
            textField.inputMethodHints: Qt.ImhFormattedNumbersOnly
            onTextChanged: {
                TimeController.maxPos = value;
                slider.inner.value = (TimeController.currentPos - TimeController.minPos) / (TimeController.maxPos - TimeController.minPos);
            }
        }
    }
}
