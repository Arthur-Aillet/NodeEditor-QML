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
        Rectangle {
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2
            color: "blue"

            ObjectLoader {
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
                id: graphicsView
                visible: true
                SplitView.fillWidth: true
            }
            StatsView {
                SplitView.preferredWidth: editView.width / 6
            }
        }
    }
}
