import QtQuick
import CutieDesigner.Nodes.Display
import org.freedesktop.gstreamer.Qt6GLVideoItem 1.0

GstGLQt6VideoItem {
    required property VideoDisplayNode node
    anchors.fill: parent
}
