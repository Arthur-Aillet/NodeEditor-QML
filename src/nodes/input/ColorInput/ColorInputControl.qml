import QtQuick
import CutieUiModule
import CutieDesignerModule

ColorPicker {
    required property ColorInputNode node

    Component.onCompleted: {
        value = node.color;
    }

    onValueChanged: {
        node.color = value;
    }
}
