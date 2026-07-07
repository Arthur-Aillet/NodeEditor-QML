import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import CutieDesigner.App.FileManager

ToolBar {
    id: topBar

    property FileManager fileManager

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    implicitHeight: 30
    RowLayout {
        spacing: 0
        ToolButton {
            text: qsTr("Load")
            onClicked: {
                topBar.fileManager.load();
            }
        }
        ToolButton {
            text: qsTr("Save")
            onClicked: {
                topBar.fileManager.save();
            }
        }
    }
}
