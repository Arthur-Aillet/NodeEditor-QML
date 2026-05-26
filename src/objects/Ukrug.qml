// Created with Qt Quick Effect Maker (version 0.44), Tue May 26 13:14:13 2026

import QtQuick

Item {
    id: rootItem

    // This is the main source for the effect
    property Item source: null

    property color baseColor: Qt.rgba(1, 1, 1, 1)
    property point point1: Qt.point(0.5, 0.5)
    property point point2: Qt.point(-0.5, 0.5)
    property point point3: Qt.point(0.5, -0.5)
    property point point4: Qt.point(-0.5, -0.5)
    property real k: 0.055
    property real scale: 0.243176
    property real smoothFactor: 0.0125699

    ShaderEffect {
        readonly property alias iSource: rootItem.source
        readonly property vector3d iResolution: Qt.vector3d(width, height, 1.0)
        readonly property alias baseColor: rootItem.baseColor
        readonly property alias point1: rootItem.point1
        readonly property alias point2: rootItem.point2
        readonly property alias point3: rootItem.point3
        readonly property alias point4: rootItem.point4
        readonly property alias k: rootItem.k
        readonly property alias scale: rootItem.scale
        readonly property alias smoothFactor: rootItem.smoothFactor

        vertexShader: 'ukrug.vert.qsb'
        fragmentShader: 'ukrug.frag.qsb'
        anchors.fill: parent
    }
}
