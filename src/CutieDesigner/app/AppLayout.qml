pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

import CutieUiModule
import CutieDesigner.Stats
import CutieDesigner.Time
import CutieDesigner.Nodes.Display

Item {
    id: appLayout
    anchors.fill: parent

    property alias objectLoader: objectLoader
    property alias sceneContent: sceneContent

    states: [
        State {
            name: ""
            StateChangeScript {
                script: {
                    if (view.needRevert) {
                        // Really not maintainable but it will do for now
                        view.graphicsView.area.inner.x -= leftPanel.width + 2;
                        view.graphicsView.area.inner.y -= topView.height + 2;
                        view.needRevert = false;
                    }
                }
            }
        },
        State {
            name: "NodesFocus"
            ParentChange {
                target: sceneContent
                parent: appLayout
            }
            ParentChange {
                target: view
                parent: appLayout
            }
            PropertyChanges {
                view {
                    graphicsView.area.background.opacity: 0.5
                    anchors.fill: appLayout
                }
                layout {
                    visible: false
                }
            }
            PropertyChanges {
                restoreEntryValues: false
                view {
                    needRevert: true
                }
            }
            StateChangeScript {
                script: {
                    view.graphicsView.area.inner.x += view.x;
                    view.graphicsView.area.inner.y += view.y;
                }
            }
        },
        State {
            name: "SurfaceFocus"
            ParentChange {
                target: sceneContent
                parent: appLayout
            }
            PropertyChanges {
                layout {
                    visible: false
                }
            }
        }
    ]

    SplitView {
        id: layout

        anchors.fill: parent
        orientation: Qt.Vertical
        Item {
            id: topView
            implicitWidth: appLayout.width
            implicitHeight: appLayout.height / 2
            Item {
                id: sceneContent
                anchors.fill: parent
                focus: sceneHoverHandler.hovered

                Keys.onPressed: event => {
                    if ((event.key == Qt.Key_Space) && (event.modifiers & Qt.ControlModifier)) {
                        if (appLayout.state != "SurfaceFocus")
                            appLayout.state = "SurfaceFocus";
                        else
                            appLayout.state = "";
                    }
                }

                HoverHandler {
                    id: sceneHoverHandler
                }

                SurfaceLoader {
                    id: objectLoader
                }
            }
        }
        SplitView {
            id: editView
            implicitWidth: appLayout.width
            implicitHeight: appLayout.height / 2
            orientation: Qt.Horizontal
            PaneBackground {
                id: leftPanel
                implicitWidth: editView.width / 6
            }
            NodeView {
                id: view

                property bool needRevert: false

                Keys.onPressed: event => {
                    if (event.key == Qt.Key_Space) {
                        if (event.modifiers & Qt.ControlModifier) {
                            if (appLayout.state != "NodesFocus") {
                                appLayout.state = "NodesFocus";
                            } else {
                                appLayout.state = "";
                            }
                        } else {
                            TimeController.playing = !TimeController.playing;
                        }
                    }
                }

                HoverHandler {
                    id: viewHoverHandler
                    onHoveredChanged: {
                        if (!view.focus && hovered)
                            view.focus = true;
                    }
                }

                SplitView.fillWidth: true
            }
            StatsView {
                implicitWidth: editView.width / 6
            }
        }
    }
}
