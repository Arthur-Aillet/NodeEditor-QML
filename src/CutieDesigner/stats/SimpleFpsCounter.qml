import QtQuick
import CutieDesigner.Stats

Text {
    width: parent.width
    FrequencyMonitor {
        id: monitor
        refreshPeriod: 500
    }
    color: "white"
    text: "Current fps: " + monitor.fps
    font.pixelSize: 12
    Image {
        x: parent.width - width
        anchors.verticalCenter: parent.verticalCenter
        width: 24
        height: 24
        source: "loading"

        RotationAnimation on rotation {
            id: rotationAnim
            duration: 12000
            from: 0
            to: 360
            paused: true
            running: true
            loops: Animation.Infinite
        }
        MouseArea {
            anchors.fill: parent
            onClicked: rotationAnim.paused = !rotationAnim.paused
        }
    }
}
