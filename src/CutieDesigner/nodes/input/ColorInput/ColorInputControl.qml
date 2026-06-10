import QtQuick
import CutieUiModule
import CutieDesigner.Nodes

ColorPicker {
    required property ColorInputNode node

    Component.onCompleted: {
        value = node.color;
    }

    onValueChanged: {
        node.color = value;
    }
}
