import QtQuick
import CutieDesigner.Nodes.Display

Item {
    id: maskShader
    required property MaskNode node

    anchors.fill: parent

    SurfaceLoader {
        id: imageLoader
        surfaceData: maskShader.node.image

        visible: false
        layer.enabled: true
    }

    SurfaceLoader {
        id: maskLoader
        surfaceData: maskShader.node.mask

        visible: false
        layer.enabled: true
    }

    ShaderEffect {
        id: shader

        visible: imageLoader.surface !== null && maskLoader.surface !== null

        anchors.fill: image

        property alias image: imageLoader
        property alias mask: maskLoader

        property int mode: maskShader.node.mode
        property bool inverted: maskShader.node.inverted
        property rect imageRect: Qt.rect(imageLoader.x, imageLoader.y, imageLoader.width, imageLoader.height)
        property rect maskRect: Qt.rect(maskLoader.x, maskLoader.y, maskLoader.width, maskLoader.height)
        property color color: maskShader.node.color
        property real spreadMin: maskShader.node.spreadMin
        property real spreadMax: maskShader.node.spreadMax
        property real tresholdMin: maskShader.node.tresholdMin
        property real tresholdMax: maskShader.node.tresholdMax

        vertexShader: 'mask.vert.qsb'
        fragmentShader: 'mask.frag.qsb'
    }
}
