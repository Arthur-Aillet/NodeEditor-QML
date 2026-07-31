import QtQuick
import CutieDesigner.Nodes.Display

ShaderEffectSource {
    anchors.fill: parent
    required property CameraNode node

    Component.onCompleted: {
        (node.cameraHandler as CameraHandler).refCount += 1;
    }

    Component.onDestruction: {
        (node.cameraHandler as CameraHandler).refCount -= 1;
    }

    sourceItem: (node.cameraHandler as CameraHandler).out
}
