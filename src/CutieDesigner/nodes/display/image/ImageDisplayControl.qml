import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import CutieDesigner.Nodes.Display

FlexboxLayout {
    id: imageDisplayControl
    direction: FlexboxLayout.FlexboxDirection.Column
    required property ImageDisplayNode node

    Button {
        text: imageDisplayControl.node.sourceFileName === "" ? "Select File" : imageDisplayControl.node.sourceFileName
        onClicked: fileDialog.open()

        FileDialog {
            id: fileDialog
            currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
            onAccepted: imageDisplayControl.node.source = selectedFile
        }
    }
    CheckBox {
        checked: imageDisplayControl.node.tile
        onCheckedChanged: {
            imageDisplayControl.node.tile = checked;
        }
        text: "tile"
    }
}
