import QtQuick

Item {
    property alias zoom: shader.zoom
    property alias translate: shader.translate
    property alias backgroundColor: shader.backgroundColor
    property alias fineGridColor: shader.fineGridColor
    property alias coarseGridColor: shader.coarseGridColor

    ShaderEffect {
        id: shader
        anchors.fill: parent

        property real zoom: 1
        property point translate: Qt.point(0., 0.)
        property color backgroundColor
        property color fineGridColor
        property color coarseGridColor

        fragmentShader: 'viewBackground.frag.qsb'
        vertexShader: 'viewBackground.vert.qsb'
    }
    anchors.fill: parent
}
