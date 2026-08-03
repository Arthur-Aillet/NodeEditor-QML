import QtQuick
import CutieDesigner.Nodes.Display

Item {
    id: dithering
    required property DitheringNode node

    anchors.fill: parent

    SurfaceLoader {
        id: loader
        surfaceData: dithering.node.surface

        visible: false
        layer.enabled: true
    }

    ShaderEffect {
        id: shader

        anchors.fill: parent

        property alias source: loader
        property rect sourceRect: Qt.rect(source.x, source.y, source.width, source.height)
        property int mode: dithering.node.mode
        property int resolution: dithering.node.resolution

        vertexShader: 'dithering.vert.qsb'
        fragmentShader: 'dithering.frag.qsb'
    }
}
