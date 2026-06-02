// Created with Qt Quick Effect Maker (version 0.44), Tue Jun 2 14:17:11 2026

import QtQuick

Item {
    id: rootItem

    // This is the main source for the effect
    property Item source: null

    property color baseColor: Qt.rgba(1, 1, 1, 1)
    property point point1: Qt.point(0, 0)
    property point point2: Qt.point(0, 0)
    property point point3: Qt.point(0, 0)
    property point point4: Qt.point(0, 0)
    property real k: 0
    property real scale: 0.25
    property real smoothFactor: 0.001
    property real fill: 1
    property bool substraction: true
    property point boxArea: Qt.point(1, 1)
    property real boxRadius: 0.1

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
        readonly property alias fill: rootItem.fill
        readonly property alias substraction: rootItem.substraction
        readonly property alias boxArea: rootItem.boxArea
        readonly property alias boxRadius: rootItem.boxRadius

        vertexShader: 'ukrug.vert.qsb'
        fragmentShader: 'ukrug.frag.qsb'
        anchors.fill: parent
    }
}
