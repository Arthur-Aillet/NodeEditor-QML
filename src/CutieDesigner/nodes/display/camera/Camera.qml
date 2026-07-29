import QtQuick
import CutieDesigner.Nodes.Display
import QtMultimedia

Item {
    id: videoDisplay
    anchors.fill: parent
    required property CameraNode node

    Component.onCompleted: {
        camera.start();
    }

    CaptureSession {
        id: captureSession
        camera: Camera {
            id: camera
        }
        videoOutput: out
    }

    VideoOutput {
        id: out
        anchors.fill: parent
    }
}
