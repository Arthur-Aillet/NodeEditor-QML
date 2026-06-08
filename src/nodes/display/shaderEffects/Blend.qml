import QtQuick
import CutieDesignerModule

Item {
    id: blend
    required property BlendNode node

    anchors.fill: parent

    SurfaceLoader {
        id: aLoader
        surfaceData: blend.node?.a ?? null
        onSurfaceChanged: {
            surface.visible = false;
            surface.layer.enabled = true;
        }
    }

    SurfaceLoader {
        id: bLoader
        surfaceData: blend.node?.b ?? null

        onSurfaceChanged: {
            surface.layer.enabled = true;
        }
    }

    ShaderEffect {
        id: shader

        anchors.fill: target

        property alias source: bLoader.surface
        property alias target: aLoader.surface

        property rect targetRect: Qt.rect(target.x, target.y, target.width, target.height)
        property rect sourceRect: Qt.rect(source.x, source.y, source.width, source.height)
        property int mode: blend.node?.mode ?? 0

        vertexShader: 'blend.vert.qsb'
        fragmentShader: 'blend.frag.qsb'
    }
}
