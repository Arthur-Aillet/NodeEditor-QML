import QtQuick
import CutieDesigner.Nodes.Display
import QtMultimedia

VideoOutput {
    id: videoDisplay
    anchors.fill: parent
    required property VideoDisplayNode node
    objectName: "videoOutput"
    Component.onCompleted: {
        node.newVideoOutput(videoDisplay);
    }

    Component.onDestruction: {
        node.removeVideoOutput(videoDisplay);
    }
}
