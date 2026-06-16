import QtQuick
import CutieDesigner.Nodes.Display

Item {
    id: blend
    required property BlendNode node

    anchors.fill: parent

    SurfaceLoader {
        id: aLoader
        surfaceData: blend.node.a

        layer.enabled: true
    }

    SurfaceLoader {
        id: bLoader
        surfaceData: blend.node.b

        layer.enabled: true
    }

    ShaderEffect {
        id: shader

        visible: aLoader.surface !== null && bLoader.surface !== null

        anchors.fill: target

        property alias source: bLoader
        property alias target: aLoader

        property rect targetRect: Qt.rect(target.x, target.y, target.width, target.height)
        property rect sourceRect: Qt.rect(source.x, source.y, source.width, source.height)
        property int mode: blend.node.mode

        vertexShader: 'blend.vert.qsb'
        fragmentShader: 'blend.frag.qsb'
    }
}
