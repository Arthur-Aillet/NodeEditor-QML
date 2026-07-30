import QtQuick
import CutieDesigner.Nodes.Display

Item {
    id: videoDisplay
    anchors.fill: parent
    required property CameraNode node

    Component.onCompleted: {
        (node.cameraHandler as CameraHandler).refCount += 1;
    }

    Component.onDestruction: {
        (node.cameraHandler as CameraHandler).refCount -= 1;
    }

    ShaderEffectSource {
        id: videooutput2
        x: 50
        y: 50
        width: 400
        height: 400
        anchors.fill: parent
        sourceItem: (videoDisplay.node.cameraHandler as CameraHandler).out
    }
}
