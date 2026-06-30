pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CutieUiModule as Cute

GridLayout {
    id: grid
    columns: 2
    columnSpacing: 20

    required property var model
    required property QtObject object

    enum ValueType {
        Slider,
        Text,
        Number,
        Color,
        CheckBox
    }

    Repeater {
        model: grid.model
        delegate: Text {
            required property int index
            required property string name
            Layout.column: 0
            Layout.row: index
            text: name + ":"
            color: palette.text
        }
    }

    Repeater {
        model: grid.model

        Component.onCompleted: {
            for (let i = 0; i != count; i++) {
                const current = itemAt(i);
                current.Layout.column = 1;
                current.Layout.row = i;

                const changeSignal = String(grid.model[i].property).charAt(0).toUpperCase() + String(grid.model[i].property).slice(1);

                current.value = grid.object[grid.model[i].property];
                current.onValueChanged.connect(() => {
                    if (current.value != grid.object[grid.model[i].property])
                        grid.object[grid.model[i].property] = current.value;
                });
                grid.object["on" + changeSignal + "Changed"].connect(() => {
                    if (current.value != grid.object[grid.model[i].property])
                        current.value = grid.object[grid.model[i].property];
                });
            }
        }
        delegate: DelegateChooser {
            id: chooser
            role: "type"

            DelegateChoice {
                roleValue: GridController.Slider
                Cute.SliderField {
                    required property int index
                    min: grid.model[index].min ?? 0.0
                    max: grid.model[index].max ?? 1.0
                    ease: grid.model[index].ease ?? SliderField.Linear
                }
            }
            DelegateChoice {
                roleValue: GridController.Number
                Cute.TextField {
                    id: numField
                    required property int index

                    textField.inputMethodHints: Qt.ImhFormattedNumbersOnly
                    textField.Layout.preferredWidth: Math.max(80, textField.contentWidth + 8)
                    textField.validator: DoubleValidator {
                        bottom: grid.model[numField.index].min ?? -Infinity
                        top: grid.model[numField.index].max ?? Infinity
                    }
                }
            }
            DelegateChoice {
                roleValue: GridController.Text
                Cute.TextField {}
            }
            DelegateChoice {
                roleValue: GridController.Color
                ColorPicker {}
            }
            DelegateChoice {
                roleValue: GridController.CheckBox
                CheckBox {
                    property bool value
                    checked: value
                    onCheckedChanged: value = checked
                }
            }
        }
    }
}
