import QtQuick
import CutieDesigner.Nodes.Display

Image {
    id: imageDisplay
    anchors.fill: parent
    required property ImageDisplayNode node
    source: node.source

    // Connections {
    //     target: imageDisplay.node
    //     function onSourceChanged() {
    //         imageDisplay.source = imageDisplay.node.source;
    //     }
    // }
}
