import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CutieDesigner.Nodes

FlexboxLayout {
    id: root
    required property StackNode node

    Button {
        text: "+"
        height: 20
        onClicked: root.node.addEmptyNode()
    }

    Button {
        text: "-"
        height: 20
        onClicked: root.node.removeLastNode()
    }
}
