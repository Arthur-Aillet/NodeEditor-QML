import QtQuick
import CutieDesigner.Nodes.Display

SurfaceLoader {
    required property DimensionNode node

    x: node.inX
    y: node.inY
    width: node.inWidth
    height: node.inHeight

    clip: true

    anchored: false
    rotation: node.rotation
    surfaceData: node.surface
}
