import QtQuick
import QtQuick.Controls
import CutieDesigner.Nodes.Process

ComboBox {
    background.implicitWidth: 80

    y: 50

    onParentChanged: {
        parent.anchors.verticalCenter = undefined;
        parent.anchors.top = parent.parent.top;
    }

    property int currentMode: CutieDesigner.ColorMode.RGBA // model[currentIndex]

    onActivated: currentMode = currentIndex

    onHighlightedIndexChanged: {
        if (highlightedIndex < 0)
            currentMode = currentIndex;
        else
            currentMode = highlightedIndex;
    }
}
