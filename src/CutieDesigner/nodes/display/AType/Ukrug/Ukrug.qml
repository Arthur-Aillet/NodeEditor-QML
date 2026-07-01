import QtQuick

ShaderEffect {
    id: ukrugShader
    property Item source: null

    property color baseColor: Qt.rgba(1, 1, 1, 1)
    property point point1: Qt.point(0, 0)
    property point point2: Qt.point(0, 0)
    property point point3: Qt.point(0, 0)
    property point point4: Qt.point(0, 0)
    property real k: 0
    property real pointsScale: 0.25
    property real smoothFactor: 0.001
    property real fill: 1
    property bool substraction: true
    property point boxArea: Qt.point(1, 1)
    property real boxRadius: 0.1
    property real circleScale: 1

    readonly property alias iSource: ukrugShader.source
    readonly property vector3d iResolution: Qt.vector3d(width, height, 1.0)

    vertexShader: 'ukrug.vert.qsb'
    fragmentShader: 'ukrug.frag.qsb'
    anchors.fill: parent
}
