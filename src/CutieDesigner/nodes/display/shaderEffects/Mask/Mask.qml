import QtQuick
import CutieDesigner.Nodes.Display

Item {
    id: mask
    required property MaskNode node

    anchors.fill: parent

    SurfaceLoader {
        id: aLoader
        surfaceData: mask.node.a

        visible: false
        layer.enabled: true
    }

    SurfaceLoader {
        id: bLoader
        surfaceData: mask.node.b

        visible: false
        layer.enabled: true
    }

    ShaderEffect {
        id: shader

        visible: aLoader.surface !== null && bLoader.surface !== null

        anchors.fill: image

        property alias image: aLoader
        property alias mask: bLoader

        property rect imageRect: Qt.rect(aLoader.x, aLoader.y, aLoader.width, aLoader.height)
        property rect maskRect: Qt.rect(bLoader.x, bLoader.y, bLoader.width, bLoader.height)

        vertexShader: 'mask.vert.qsb'
        fragmentShader: 'mask.frag.qsb'
    }
}
