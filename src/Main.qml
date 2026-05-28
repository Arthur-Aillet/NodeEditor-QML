import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NodeModule
import StatsModule

ApplicationWindow {
    id: root
    width: 1000
    height: 800
    visible: true
    title: qsTr("CutieDesigner!")

    signal newRootComponent(newValue: Component)
    signal removeComponent

    onRemoveComponent: {
        source.sourceComponent = undefined;
    }

    onNewRootComponent: newValue => {
        source.sourceComponent = newValue;
    }

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
            Loader {
                id: source
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
