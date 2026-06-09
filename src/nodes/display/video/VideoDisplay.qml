import QtQuick
import QtMultimedia
import CutieDesignerModule

Item {
    required property VideoDisplayNode node

    VideoOutput {
        id: preview2

        layer.enabled: true
        fillMode: VideoOutput.Stretch

        x: 100
        y: 100
        width: 100
        height: 100
        CaptureSession {
            id: captureSession2
            camera: Camera {
                id: cam2
            }
            imageCapture: ImageCapture {
                id: imageCapture2
            }

            recorder: MediaRecorder {
                id: recorder2
            }
            videoOutput: preview2

            Component.onCompleted: {
                cam2.start();
            }
        }
    }
    VideoOutput {
        id: preview

        x: 0
        y: 0
        width: 100
        height: 100

        fillMode: VideoOutput.Stretch
        layer.enabled: true

        CaptureSession {
            id: captureSession
            camera: Camera {
                id: cam
            }
            imageCapture: ImageCapture {
                id: imageCapture
            }

            recorder: MediaRecorder {
                id: recorder
            }
            videoOutput: preview

            Component.onCompleted: {
                //cam.start();
            }
        }
    }
}
