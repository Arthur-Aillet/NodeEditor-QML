import QtQuick
import CutieDesignerModule

Item {
    id: blend
    required property BlendNode node

    anchors.fill: parent

    SurfaceLoader {
        id: aLoader
        surfaceData: blend.node.a
        onSurfaceChanged: {
            surface.visible = bLoader.surface === null;
            surface.layer.enabled = true;
        }
    }

    SurfaceLoader {
        id: bLoader
        surfaceData: blend.node.b

        onSurfaceChanged: {
            surface.layer.enabled = true;
        }
    }

    ShaderEffect {
        id: shader

        visible: source !== null && target !== null
        anchors.fill: target

        property alias source: bLoader.surface
        property alias target: aLoader.surface

        property rect targetRect: target !== null ? Qt.rect(target.x, target.y, target.width, target.height) : Qt.rect(0, 0, 0, 0)
        property rect sourceRect: source !== null ? Qt.rect(source.x, source.y, source.width, source.height) : Qt.rect(0, 0, 0, 0)
        property int mode: blend.node.mode

        vertexShader: 'blend.vert.qsb'
        fragmentShader: 'blend.frag.qsb'
    }
}
