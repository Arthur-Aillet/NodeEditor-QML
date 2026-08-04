import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CutieDesigner.Nodes.Display

FlexboxLayout {
    id: maskControl
    direction: FlexboxLayout.FlexboxDirection.Column
    required property MaskNode node

    ComboBox {

        model: ["Opacity", "Lightness", "Color", "Hue"]

        Component.onCompleted: {
            currentIndex = maskControl.node.mode;
        }

        onActivated: {
            maskControl.node.mode = currentIndex;
        }

        onHighlightedIndexChanged: {
            if (highlightedIndex < 0)
                return;
            else
                maskControl.node.mode = highlightedIndex;
        }
    }

    CheckBox {
        checked: maskControl.node.inverted
        text: qsTr("inverted")
        onCheckedChanged: maskControl.node.inverted = checked
    }
}
