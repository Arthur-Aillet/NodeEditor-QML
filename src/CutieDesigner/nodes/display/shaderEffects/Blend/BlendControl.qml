import QtQuick
import QtQuick.Controls
import CutieDesigner.Nodes.Display

ComboBox {
    required property BlendNode node

    model: ["Addition", "Average", "Color", "Color Burn", "Color Dodge", "Darken", "Darker Color", "Difference", "Divide", "Exclusion", "Hard Light", "Hue", "Lighten", "Lighter Color", "Lightness", "Negation", "Multiply", "Saturation", "Screen", "Subtract", "Soft Light", "Broken"]

    onActivated: node.mode = currentIndex

    onHighlightedIndexChanged: {
        if (highlightedIndex < 0)
            return;
        if (highlightedIndex == (count - 1))
            node.mode = -1;
        else
            node.mode = highlightedIndex;
    }
}
