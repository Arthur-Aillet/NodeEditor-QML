import QtQuick
import QtQuick.Controls
import CutieDesigner.Nodes.Display

ComboBox {
    required property DitheringNode node

    model: ["4x4 Bayer", "8x8 Bayer"]

    Component.onCompleted: {
        currentIndex = node.mode;
    }

    onActivated: node.mode = currentIndex

    onHighlightedIndexChanged: {
        if (highlightedIndex < 0)
            return;
        node.mode = highlightedIndex;
    }
}
