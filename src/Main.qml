pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NodeModule
import CutieDesignerModule
import StatsModule

ApplicationWindow {
    id: root
    width: 1000
    height: 800
    visible: true
    color: "black"
    title: qsTr("CutieDesigner!")

    signal valueUpdated(newValue: string)
    property alias objectLoader: objectLoader

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
        }
    }
    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical
        Item {
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2

            SurfaceLoader {
                id: objectLoader
            }
        }
        SplitView {
            id: editView
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2
            orientation: Qt.Horizontal
            PaneBackground {
                SplitView.preferredWidth: editView.width / 6
            }
            GraphicsView {
                visible: true
                SplitView.fillWidth: true
            }
            StatsView {
                SplitView.preferredWidth: editView.width / 6
            }
        }
    }
}
