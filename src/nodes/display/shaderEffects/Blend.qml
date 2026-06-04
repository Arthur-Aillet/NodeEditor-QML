// Created with Qt Quick Effect Maker (version 0.44), Thu Jun 4 11:49:43 2026

import QtQuick

ShaderEffect {
    id: shader
    required property Item source
    required property Item target

    property rect targetRect: Qt.rect(target.x, target.y, target.width, target.height)
    property rect sourceRect: Qt.rect(source.x, source.y, source.width, source.height)
    property int mode: 0

    Timer {
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            shader.mode += 1;
            if (shader.mode == 22)
                shader.mode = 0;
        }
    }

    vertexShader: 'nodes/display/shaderEffects/blend.vert.qsb'
    fragmentShader: 'nodes/display/shaderEffects/blend.frag.qsb'
}
