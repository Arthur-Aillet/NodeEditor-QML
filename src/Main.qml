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

            SurfaceLoader {
                id: objectLoader
            }

            Rectangle {
                id: a
                x: 100
                y: 100
                width: 100
                height: 100

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: "lightgreen"
                    }
                    GradientStop {
                        position: 1.0
                        color: "green"
                    }
                }
                layer.enabled: true
            }

            Rectangle {
                id: b
                y: 150
                width: 100
                height: 100

                NumberAnimation on x {
                    loops: Animation.Infinite
                    duration: 2000
                    from: 100
                    to: 200
                }
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop {
                        position: 0.0
                        color: "pink"
                    }
                    GradientStop {
                        position: 1.0
                        color: "red"
                    }
                }

                visible: false
                layer.enabled: true
            }
            Blend {
                anchors.fill: b
                source: b
                target: a
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
