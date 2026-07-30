import QtQuick
import CutieDesigner.Nodes.Display
import QtMultimedia

Video {
    id: videoDisplay
    anchors.fill: parent
    required property VideoDisplayNode node
    source: node.source

    onSourceChanged: {
        console.log("Source changed");
        if (source) {
            videoDisplay.play();
            console.log("PLay");
        }
    }

    playbackRate: 1
    loops: node.looping ? MediaPlayer.Infinite : 1

    Component.onCompleted: {
        console.log("completed");
    }
}
