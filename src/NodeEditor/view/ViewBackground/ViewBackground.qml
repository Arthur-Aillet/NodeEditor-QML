import QtQuick

Item {
    ShaderEffect {
        anchors.fill: parent

        fragmentShader: 'viewBackground.frag.qsb'
        vertexShader: 'viewBackground.vert.qsb'
    }
    anchors.fill: parent
}
