import QtQuick
import CutieDesigner.Nodes

SurfaceLoader {
    id: dimension
    required property DimensionNode node

    x: node.inX
    y: node.inY
    width: node.inWidth
    height: node.inHeight

    clip: true

    anchored: false

    surfaceData: dimension.node.surface
}
