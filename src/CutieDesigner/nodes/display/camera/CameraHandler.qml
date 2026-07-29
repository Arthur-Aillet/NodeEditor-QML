import QtQuick
import QtMultimedia

Item {
    property alias camera: camera
    property alias out: out

    CaptureSession {
        camera: Camera {
            id: camera
        }
        videoOutput: out
    }

    VideoOutput {
        id: out
        width: 1280
        height: 720
        opacity: 0
    }
}
