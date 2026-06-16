pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

import NodeEditor
import CutieDesigner.Stats
import CutieDesigner.Nodes

Item {
    id: root
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
                        view.area.inner.x -= leftPanel.width + 2;
                        view.area.inner.y -= topView.height + 2;
                        view.needRevert = false;
                    }
                }
            }
        },
        State {
            name: "NodesFocus"
            ParentChange {
                target: sceneContent
                parent: root
            }
            ParentChange {
                target: view
                parent: root
            }
            PropertyChanges {
                view {
                    SplitView.preferredWidth: undefined
                    area.background.opacity: 0.5
                    anchors.fill: root
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
                    view.area.inner.x += view.x;
                    view.area.inner.y += view.y;
                }
            }
        },
        State {
            name: "SurfaceFocus"
            ParentChange {
                target: sceneContent
                parent: root
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
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2
            Item {
                id: sceneContent
                anchors.fill: parent
                focus: sceneHoverHandler.hovered

                Keys.onPressed: event => {
                    if ((event.key == Qt.Key_Space) && (event.modifiers & Qt.ControlModifier)) {
                        if (root.state != "SurfaceFocus")
                            root.state = "SurfaceFocus";
                        else
                            root.state = "";
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
            SplitView.preferredWidth: root.width
            SplitView.preferredHeight: root.height / 2
            orientation: Qt.Horizontal
            PaneBackground {
                id: leftPanel
                SplitView.preferredWidth: editView.width / 6
            }
            GraphicsView {
                id: view

                property bool needRevert: false

                Keys.onPressed: event => {
                    if ((event.key == Qt.Key_Space) && (event.modifiers & Qt.ControlModifier)) {
                        if (root.state != "NodesFocus") {
                            root.state = "NodesFocus";
                        } else {
                            root.state = "";
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
                SplitView.preferredWidth: editView.width / 6
            }
        }
    }
}
