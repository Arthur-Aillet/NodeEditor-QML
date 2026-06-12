import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CutieUiModule
import CutieDesigner.Nodes

FlexboxLayout {
    id: gradientInputControl

    required property GradientInputNode node

    onParentChanged: {
        parent.anchors.verticalCenter = undefined;
        parent.anchors.top = parent.parent.top;
    }

    y: 10

    direction: FlexboxLayout.FlexboxDirection.Column
    alignItems: FlexboxLayout.FlexboxAlignment.AlignCenter
    gap: 4

    FlexboxLayout {
        Button {
            text: "+"
            implicitHeight: 20
            implicitWidth: 30
            //onClicked: stackControl.node.addEmptyPort()
        }

        Button {
            text: "-"
            implicitHeight: 20
            implicitWidth: 30
            //onClicked: stackControl.node.removeLastPort()
        }
        y: 10
    }

    ColorPicker {

        Component.onCompleted: {
            value = gradientInputControl.node.color;
        }

        onValueChanged: {
            gradientInputControl.node.color = value;
        }
    }
}
