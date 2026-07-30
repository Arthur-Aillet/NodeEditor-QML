import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import CutieDesigner.Nodes.Display

FlexboxLayout {
    id: videoDisplayControl
    direction: FlexboxLayout.FlexboxDirection.Column
    required property VideoDisplayNode node

    Button {
        text: videoDisplayControl.node.sourceFileName === "" ? "Select File" : videoDisplayControl.node.sourceFileName
        onClicked: fileDialog.open()

        FileDialog {
            id: fileDialog
            currentFolder: StandardPaths.standardLocations(StandardPaths.MoviesLocation)[0]
            onAccepted: videoDisplayControl.node.source = selectedFile
        }
    }

    CheckBox {
        checked: videoDisplayControl.node.looping
        text: qsTr("looping")
    }
}
