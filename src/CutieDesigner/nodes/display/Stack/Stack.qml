import QtQuick
import CutieDesigner.Nodes.Display
import CutieDesigner.Datatypes

Item {
    id: stack
    required property StackNode node

    anchors.fill: parent

    Repeater {
        model: stack.node.surfaceList
        SurfaceLoader {
            required property SurfaceData display
            surfaceData: display
        }
    }
}
