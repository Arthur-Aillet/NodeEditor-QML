pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NodeEditor
import CutieDesigner.Nodes
import CutieDesigner.Stats

ApplicationWindow {
    id: root
    width: 1400
    height: 1000
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
    Item {
        id: rootItem
        anchors.fill: parent

        states: [
            State {
                name: ""
                StateChangeScript {
                    script: {
                        if (view.needRevert) {
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
                    parent: rootItem
                }
                ParentChange {
                    target: view
                    parent: rootItem
                }
                PropertyChanges {
                    view {
                        SplitView.preferredWidth: undefined
                        area.background.opacity: 0.5
                        anchors.fill: rootItem
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
                    parent: rootItem
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
                            if (rootItem.state != "SurfaceFocus")
                                rootItem.state = "SurfaceFocus";
                            else
                                rootItem.state = "";
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
                            if (rootItem.state != "NodesFocus")
                                rootItem.state = "NodesFocus";
                            else
                                rootItem.state = "";
                        }
                    }

                    HoverHandler {
                        id: viewHoverHandler
                        onHoveredChanged: {
                            if (view.focus == false)
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
}
