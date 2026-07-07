import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

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
                if (!topBar.fileManager.graphEmpty()) {
                    overwriteDialog.open();
                } else {
                    loadFileDialog.overwrite = false;
                    loadFileDialog.open();
                }
            }
        }
        ToolButton {
            text: qsTr("Save")
            onClicked: {
                saveFileDialog.open();
            }
        }
    }

    FileDialog {
        id: saveFileDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Cute Graph (*.cute)", "Json (*.json)", "Any (*)"]
        defaultSuffix: "cute"
        onAccepted: {
            const errorMessage = topBar.fileManager.saveGraph(selectedFile);

            if (errorMessage != "") {
                errorDialog.text = errorMessage;
                errorDialog.open();
            }
        }
    }

    FileDialog {
        id: loadFileDialog
        fileMode: FileDialog.OpenFile
        defaultSuffix: saveFileDialog.defaultSuffix
        nameFilters: saveFileDialog.nameFilters
        property bool overwrite: false
        onAccepted: {
            const errorMessage = topBar.fileManager.loadGraph(selectedFile, overwrite);
            overwrite = false;

            if (errorMessage != "") {
                errorDialog.text = errorMessage;
                errorDialog.open();
            }
        }
    }

    MessageDialog {
        id: errorDialog
        title: "Error Occured"
        buttons: Dialog.Accepted
        popupType: Popup.Window
    }

    MessageDialog {
        id: overwriteDialog
        popupType: Popup.Window
        text: "The current graph isn't empty."
        informativeText: "Do you want to erase it before loading a new graph?"
        buttons: MessageDialog.Yes | MessageDialog.No
        onAccepted: {
            loadFileDialog.overwrite = true;
            loadFileDialog.open();
        }
        onRejected: {
            loadFileDialog.overwrite = false;
            loadFileDialog.open();
        }
    }
}
