import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CutieDesigner.Nodes

FlexboxLayout {
    id: stackControl
    required property StackNode node

    onParentChanged: {
        parent.anchors.verticalCenter = undefined;
        parent.anchors.top = parent.parent.top;
    }

    Button {
        text: "+"
        implicitHeight: 20
        implicitWidth: 30
        onClicked: stackControl.node.addEmptyPort()
    }

    Button {
        text: "-"
        implicitHeight: 20
        implicitWidth: 30
        onClicked: stackControl.node.removeLastPort()
    }
    y: 10
}
