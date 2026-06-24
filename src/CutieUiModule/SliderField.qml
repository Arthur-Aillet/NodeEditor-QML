pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CutieUiModule as Cute

Cute.TextField {
    id: root

    property double min: 0
    property double max: 1
    property int ease: SliderField.Linear

    property bool set: false

    enum Type {
        Linear,
        Sine,
        Quad,
        Cubic,
        Quart,
        Exp
    }

    onValueChanged: {
        if (!set) {
            slider.value = root.value / root.max;
            value = Number(value).toFixed(2);
            set = true;
        }
    }

    textField.onEditingFinished: {
        value = text;
    }

    textField.validator: DoubleValidator {
        bottom: root.min
        top: root.max
    }

    Slider {
        id: slider
        x: 0
        from: 0
        to: 1
        stepSize: 0.01
        Layout.fillWidth: true
        Layout.fillHeight: true

        function lerp(factor: double): double {
            return root.min + (root.max - root.min) * factor;
        }

        function easeInvFunction(x: double): double {
            switch (root.ease) {
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
            switch (root.ease) {
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

        onMoved: {
            root.value = lerp(easeInFunction(value)).toFixed(2);
        }

        background: Rectangle {
            x: slider.leftPadding + 5
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            implicitWidth: 80
            implicitHeight: 4
            width: slider.availableWidth - 10
            height: implicitHeight
            radius: 2
            color: "#bdbebf"
            Repeater {
                model: 9
                Rectangle {
                    required property int index
                    height: 4
                    width: 0.75
                    antialiasing: true
                    z: -1
                    color: "grey"
                    x: {
                        return slider.background.width * slider.easeInvFunction(0.1 * index + 0.1);
                    }
                    y: 3
                }
            }

            Rectangle {
                width: slider.visualPosition * parent.width
                height: parent.height
                color: Qt.rgba(1, 0.647058824, 0, 1)
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
