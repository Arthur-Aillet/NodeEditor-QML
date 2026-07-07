import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    id: topBar
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    implicitHeight: 30
    RowLayout {
        spacing: 0
        ToolButton {
            text: qsTr("Load")
        }
        ToolButton {
            text: qsTr("Save")
        }
    }
}
