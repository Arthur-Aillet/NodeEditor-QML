import QtQuick
import CutieUiModule
import CutieDesigner.Nodes.Input

ColorPicker {
    required property ColorInputNode node

    Component.onCompleted: {
        value = node.color;
    }

    onValueChanged: {
        node.color = value;
    }
}
