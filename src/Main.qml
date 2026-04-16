import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GraphModule

Window {
    id: root
    width: 800
    height: 800
    visible: true
    title: qsTr("CutieDesigner!")
    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical
        Rectangle {
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2
            color: "blue"
        }
        Item {
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2
            GraphView {}
        }
    }
}
