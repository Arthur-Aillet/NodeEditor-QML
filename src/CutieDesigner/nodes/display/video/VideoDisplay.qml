pragma ComponentBehavior: Bound
import QtQuick
import CutieDesigner.Nodes.Display

Item {
    id: videoDisplay
    anchors.fill: parent
    required property VideoDisplayNode node
    //layer.enabled: true
    //layer.live: true

    Component.onDestruction: {
        console.log("qml side destruction");
    }
}
