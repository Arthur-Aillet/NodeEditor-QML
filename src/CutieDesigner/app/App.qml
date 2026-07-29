import QtQuick

import CutieDesigner.App
import CutieDesigner.Nodes.Display
import CutieDesigner.App.FileManager
import CutieUiModule

CutieWindow {
    id: cutieWindow
    width: 1400
    height: 1000
    visible: true
    color: "black"
    title: qsTr("CutieDesigner!")

    property alias objectLoader: appLayout.objectLoader
    property alias sceneContent: appLayout.sceneContent
    property alias cameraHandler: cameraHandler

    property FileManager fileManager

    CameraHandler {
        id: cameraHandler
    }

    TopBar {
        id: topBarId
        fileManager: cutieWindow.fileManager
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 30
    }

    AppLayout {
        id: appLayout
        topBar: topBarId
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topBar.bottom
        anchors.bottom: parent.bottom
    }
}
