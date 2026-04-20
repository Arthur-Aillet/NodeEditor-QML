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
    // NodeEditor {
    //     id: nodeEditor
    //     width: 800
    //     height: 800
    // }
    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("Swap")
                onClicked: {
                    if (nodeEditor.visible) {
                        nodeEditor.visible = false;
                        graphicsView.visible = true;
                    } else {
                        nodeEditor.visible = true;
                        graphicsView.visible = false;
                    }
                }
            }
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
            NodeEditor {
                id: nodeEditor
                visible: false
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
