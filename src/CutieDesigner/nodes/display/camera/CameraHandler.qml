import QtQuick
import QtMultimedia

Item {
    id: handler
    property alias out: out

    property int refCount: 0

    onRefCountChanged: () => {
        if (refCount == 0) {
            camera.stop();
        }
        if (refCount > 0 && !camera.active) {
            camera.start();
        }
    }

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
