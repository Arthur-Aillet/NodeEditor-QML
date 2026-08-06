import QtQuick
import CutieDesigner.Nodes.Display

SurfaceLoader {
    required property DimensionNode node

    x: node.inPos.x
    y: node.inPos.y
    width: node.inSize.x
    height: node.inSize.y

    clip: true

    anchored: false
    rotation: node.rotation
    surfaceData: node.surface
}
