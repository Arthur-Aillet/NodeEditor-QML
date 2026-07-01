pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FlexboxLayout {
    id: sliderField
    direction: FlexboxLayout.Row
    alignItems: FlexboxLayout.AlignCenter
    justifyContent: FlexboxLayout.JustifyStart
    gap: 6

    property double min: 0
    property double max: 1
    property double value: 0
    property real decimal: 2
    property alias inner: slider
    property int ease: SliderField.Linear

    enum Type {
        Linear,
        Sine,
        Quad,
        Cubic,
        Quart,
        Exp
    }

    onValueChanged: {
        slider.value = slider.easeInvFunction((value - min) / (max - min));
        textField.text = Number(value).toFixed(sliderField.decimal);
    }

    TextField {
        id: textField
        Layout.preferredWidth: Math.max(40, contentWidth + 8)
        text: {
            text = sliderField.value.toFixed(sliderField.decimal);
        }
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        onEditingFinished: {
            sliderField.value = Math.max(Math.min(Number(text), sliderField.max), sliderField.min).toFixed(sliderField.decimal);
        }
    }

    Slider {
        id: slider
        x: 0
        from: 0
        to: 1
        snapMode: Slider.NoSnap
        stepSize: ((1 / 10) ** sliderField.decimal) / (sliderField.max - sliderField.min)
        Layout.fillWidth: true
        Layout.fillHeight: true

        function lerp(factor: double): double {
            return sliderField.min + (sliderField.max - sliderField.min) * factor;
        }

        function easeInvFunction(x: double): double {
            switch (sliderField.ease) {
            case SliderField.Linear:
                return x;
            case SliderField.Sine:
                return Math.sin((x * Math.PI) / 2);
            case SliderField.Quad:
                return Math.pow(x, 1 / 2);
            case SliderField.Cubic:
                return Math.pow(x, 1 / 3);
            case SliderField.Quart:
                return Math.pow(x, 1 / 4);
            case SliderField.Exp:
                return (Math.log2(x) + 10) / 10;
            default:
                return x;
            }
        }

        function easeInFunction(x: double): double {
            switch (sliderField.ease) {
            case SliderField.Linear:
                return x;
            case SliderField.Sine:
                return 1 - Math.cos((x * Math.PI) / 2);
            case SliderField.Quad:
                return x * x;
            case SliderField.Cubic:
                return x * x * x;
            case SliderField.Quart:
                return x * x * x * x;
            case SliderField.Exp:
                return x === 0 ? 0 : Math.pow(2, 10 * x - 10);
            default:
                return x;
            }
        }

        onMoved: sliderField.value = lerp(easeInFunction(visualPosition))

        background: Rectangle {
            x: slider.leftPadding + 5
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            implicitWidth: 80
            implicitHeight: 4
            width: slider.availableWidth - 10
            height: implicitHeight
            radius: 2
            color: Qt.lighter(palette.mid, 1 + 0.2 * sliderHoverHandler.hovered + 0.2 * parent.focus)

            HoverHandler {
                id: sliderHoverHandler
                margin: 10
            }

            Repeater {
                model: 9
                Rectangle {
                    required property int index
                    height: 4
                    width: 0.75
                    antialiasing: true
                    z: -1
                    color: palette.buttonText
                    x: {
                        return slider.background.width * slider.easeInvFunction(0.1 * index + 0.1);
                    }
                    y: 3
                }
            }

            Rectangle {
                width: slider.visualPosition * parent.width
                height: parent.height
                color: palette.highlight
                radius: 2
            }
        }

        handle: Rectangle {
            x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            implicitWidth: 12
            implicitHeight: 12
            radius: 13
            color: slider.pressed ? "#f0f0f0" : "#f6f6f6"
            border.color: "#bdbebf"
        }
    }
}
