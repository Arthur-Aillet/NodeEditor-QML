pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import CutieDesigner.App

ApplicationWindow {
    width: 1400
    height: 1000
    visible: true
    color: "black"
    title: qsTr("CutieDesigner!")

    property alias objectLoader: appLayout.objectLoader

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
        }
    }
    AppLayout {
        id: appLayout
    }
}
