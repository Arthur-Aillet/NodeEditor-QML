import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NodeModule

ApplicationWindow {
    id: root
    width: 800
    height: 800
    visible: true
    title: qsTr("CutieDesigner!")
    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
        }
    }
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
                color: "green"
            }
            GraphicsView {
                id: graphicsView
                visible: true
                SplitView.fillWidth: true
            }
            Rectangle {
                SplitView.preferredWidth: editView.width / 6
                color: "purple"
                opacity: 0
            }
        }
    }
}
