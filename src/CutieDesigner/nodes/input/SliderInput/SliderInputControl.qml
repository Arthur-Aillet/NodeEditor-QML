import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CutieDesigner.Nodes.Input
import CutieUiModule

FlexboxLayout {
    id: sliderInputControl

    required property SliderInputNode node

    direction: FlexboxLayout.Row
    alignItems: FlexboxLayout.AlignCenter
    gap: 6

    TextField {
        value: {
            value = sliderInputControl.node.min;
        }
        onValueChanged: {
            sliderInputControl.node.min = value;
        }
        textField.validator: DoubleValidator {}
    }
    TextField {
        value: {
            value = sliderInputControl.node.max;
        }
        onValueChanged: {
            sliderInputControl.node.max = value;
        }
        textField.validator: DoubleValidator {}
    }

    SliderField {
        direction: FlexboxLayout.RowReverse
        min: sliderInputControl.node.min
        max: sliderInputControl.node.max
        onValueChanged: {
            sliderInputControl.node.value = value;
        }
    }
}
