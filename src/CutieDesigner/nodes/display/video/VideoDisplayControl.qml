import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import CutieDesigner.Nodes.Display

Button {
    id: videoDisplayControl
    required property VideoDisplayNode node
    text: node.sourceFileName === "" ? "Select File" : node.sourceFileName
    onClicked: fileDialog.open()

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.MoviesLocation)[0]
        onAccepted: videoDisplayControl.node.source = selectedFile
    }
}
