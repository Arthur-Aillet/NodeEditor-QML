import QtQuick
import CutieDesigner.Stats

Text {
    width: parent.width
    FrequencyMonitor {
        id: monitor
        refreshPeriod: 1000
    }
    color: palette.text
    text: "Current fps: " + monitor.fps
    font.pixelSize: 12
}
