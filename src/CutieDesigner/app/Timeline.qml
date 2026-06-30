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
            onClicked: {
                TimeController.playing = !TimeController.playing;
                focus = false;
            }
        }

        MediaButton {
            type: MediaButton.MediaButtonType.Stop
            onClicked: {
                TimeController.stop();
                focus = false;
            }
        }

        MediaButton {
            type: MediaButton.MediaButtonType.Loop
            checkable: true
            checked: TimeController.looping
            onClicked: {
                TimeController.looping = !TimeController.looping;
                focus = false;
            }
        }

        SliderField {
            id: slider
            direction: FlexboxLayout.RowReverse
            min: TimeController.minFrame
            max: TimeController.maxFrame
            decimal: 0
            onValueChanged: {
                TimeController.currentFrame = value;
            }
            inner.onMoved: {
                TimeController.playing = false;
            }
            Connections {
                target: TimeController
                function onCurrentFrameChanged() {
                    slider.value = TimeController.currentFrame;
                }
            }
            Layout.fillWidth: true
        }
        TextField {
            value: Number(TimeController.currentTime).toFixed(2) + 's'
            enabled: false
            textField.placeholderText: "Time (s)"
            textField.inputMethodHints: Qt.ImhDigitsOnly
        }
        Text {
            text: "Start: "
            color: "white"
        }
        TextField {
            value: {
                value = TimeController.minFrame;
            }
            textField.placeholderText: "Min Frames"
            textField.inputMethodHints: Qt.ImhDigitsOnly
            onTextChanged: {
                TimeController.minFrame = Math.round(value);
                slider.inner.value = (TimeController.currentFrame - TimeController.minFrame) / (TimeController.maxFrame - TimeController.minFrame);
            }
        }
        Text {
            text: "End: "
            color: "white"
        }
        TextField {
            value: {
                value = TimeController.maxFrame;
            }
            textField.placeholderText: "Max Frames"
            textField.inputMethodHints: Qt.ImhDigitsOnly
            onTextChanged: {
                TimeController.maxFrame = Math.round(value);
                slider.inner.value = (TimeController.currentFrame - TimeController.minFrame) / (TimeController.maxFrame - TimeController.minFrame);
            }
        }
    }
}
