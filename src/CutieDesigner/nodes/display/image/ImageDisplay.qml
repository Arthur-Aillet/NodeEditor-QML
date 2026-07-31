import QtQuick
import CutieDesigner.Nodes.Display

Item {
    id: imageDisplay
    anchors.fill: parent
    required property ImageDisplayNode node
    clip: true

    Image {
        id: img
        source: imageDisplay.node.source
        asynchronous: true

        fillMode: imageDisplay.node.tile ? Image.Tile : Image.Pad

        // property real sizeX: (882 * imageDisplay.node.scale.x)
        // property real sizeY: (400 * imageDisplay.node.scale.y)
        // property real realtiveX: imageDisplay.node.tile ? (imageDisplay.node.pos.x % sizeX - (imageDisplay.node.pos.x > 0) * sizeX) : imageDisplay.node.pos.x
        // property real realtiveY: imageDisplay.node.tile ? (imageDisplay.node.pos.y % sizeY - (imageDisplay.node.pos.y > 0) * sizeY) : imageDisplay.node.pos.y

        property real sizeX: imageDisplay.node.sizeSet ? imageDisplay.node.size.x : sourceSize.width
        property real sizeY: imageDisplay.node.sizeSet ? imageDisplay.node.size.y : sourceSize.height
        property real facX: imageDisplay.node.sizeSet ? imageDisplay.node.size.x / sourceSize.width : 1
        property real facY: imageDisplay.node.sizeSet ? imageDisplay.node.size.y / sourceSize.height : 1
        property real realtiveX: imageDisplay.node.tile ? (imageDisplay.node.pos.x % sizeX - (imageDisplay.node.pos.x > 0) * sizeX) : imageDisplay.node.pos.x
        property real realtiveY: imageDisplay.node.tile ? (imageDisplay.node.pos.y % sizeY - (imageDisplay.node.pos.y > 0) * sizeY) : imageDisplay.node.pos.y

        horizontalAlignment: Image.AlignLeft
        verticalAlignment: Image.AlignTop

        x: realtiveX
        y: realtiveY
        width: (parent.width - realtiveX) / facX + 1 // + 1 to hide any rounding error
        height: (parent.height - realtiveY) / facY + 1
        transform: [
            Scale {
                xScale: img.facX
                yScale: img.facY
            }
        ]
    }
}
