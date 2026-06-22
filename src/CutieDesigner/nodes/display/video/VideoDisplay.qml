pragma ComponentBehavior: Bound
import QtQuick
import CutieDesigner.Nodes.Display
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

Item {
    id: videoDisplay
    anchors.fill: parent
    required property VideoDisplayNode node
    property alias videoItem: videoItem

    GstGLQt6VideoItem {
        id: videoItem
        width: 1280
        height: 720
        transform: Scale {
            id: s
            xScale: videoDisplay.width / 1280
            yScale: videoDisplay.height / 720
        }
    }
}
