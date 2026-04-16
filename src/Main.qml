import QtQuick
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
        SplitView {
            id: editView
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2
            orientation: Qt.Horizontal
            Rectangle {
                SplitView.preferredWidth: editView.width / 6
                SplitView.preferredHeight: editView.height
                color: "green"
            }
            GraphView {
                SplitView.preferredWidth: editView.width * 4 / 6
                SplitView.preferredHeight: editView.height
            }
            Rectangle {
                SplitView.preferredWidth: editView.width / 6
                SplitView.preferredHeight: editView.height
                color: "purple"
            }
        }
    }
}
