import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import CutieDesigner.Nodes.Display

Button {
    id: imageDisplayControl
    required property ImageDisplayNode node
    text: node.sourceFileName === "" ? "Select File" : node.sourceFileName
    onClicked: fileDialog.open()

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: imageDisplayControl.node.source = selectedFile
    }
}
