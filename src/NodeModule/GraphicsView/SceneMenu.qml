pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeModule

Menu {
    id: root
    popupType: Popup.Window
    focus: true
    property var nodeMap: DataFlowModelInterface.registery.nodeMapModel

    TextField {
        id: searchField
        placeholderText: qsTr("Filter")
        Component.onCompleted: () => {
            searchField.forceActiveFocus(Qt.PopupFocusReason);
        }
        onTextChanged: () => {
            DataFlowModelInterface.registery.filterNodeMapModel(text);
            view.expandRecursively();
        }
    }

    component NodeItem: QtObject {
        property string display
        property var idx
    }

    TreeView {
        id: view
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            active: ScrollBar.AlwaysOn
        }

        clip: true
        width: parent.width
        anchors.top: searchField.bottom
        anchors.bottom: parent.bottom
        boundsBehavior: Flickable.StopAtBounds
        editTriggers: TableView.NoEditTriggers

        model: root.nodeMap
        keyNavigationEnabled: true
        selectionModel: ItemSelectionModel {}
        delegate: TreeViewDelegate {
            id: delegate
            contentItem: Label {
                id: label
                x: padding + (delegate.isTreeNode ? (delegate.depth + 1) * delegate.indentation : 0)
                topPadding: 1
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - padding - x
                clip: true
                color: (!delegate.hasChildren && delegate.row === view.currentRow) ? "white" : "black"
                text: root.underlineSearch(delegate.model.display)
            }

            background: Rectangle {
                id: background
                height: parent.height
                width: root.width
                color: (view.alternatingRows && delegate.row % 2 !== 0) ? "white" : "lightgrey"
                Rectangle {
                    x: delegate.hasChildren ? delegate.padding + (delegate.isTreeNode ? (delegate.depth + 1) * delegate.indentation : 0) + 4 : 0
                    width: parent.width - delegate.padding - x
                    height: parent.height
                    color: Qt.rgba(palette.highlight.r, palette.highlight.g, palette.highlight.b, delegate.hasChildren ? 0.2 : 1)
                    radius: delegate.hasChildren ? 4 : 0
                    border.color: palette.highlight
                    border.width: delegate.hasChildren ? 1 : 0

                    opacity: delegate.row === view.currentRow ? 1 : 0
                }
            }

            focus: delegate.row === view.currentRow

            function activate() {
                if (delegate.hasChildren) {
                    view.toggleExpanded(delegate.row);
                    if (delegate.expanded) {
                        let modelIndex = treeView.modelIndex(Qt.point(0, delegate.row));

                        let nextModelIndex = root.nodeMap.sibling(modelIndex.row + 1, 0, modelIndex);
                        let nextViewRow = treeView.rowAtIndex(nextModelIndex);
                        if (nextViewRow == -1) {
                            view.contentY = view.contentHeight;
                        } else {
                            view.positionViewAtRow(nextViewRow - 1, TreeView.Contain);
                        }
                    }
                } else {
                    console.log("Create + " + delegate.model.display);
                }
            }
            onClicked: activate()
            Keys.onEnterPressed: activate()
        }
        Component.onCompleted: expandRecursively()
    }

    function underlineSearch(text: string): string {
        const escaped = searchField.text.replace(/[\\\.\+\*\?\^\$\[\]\(\)\{\}\/\'\#\:\!\=\|]/ig, "\\$&");
        const re = new RegExp(escaped, 'gi'); // global, insensitive
        return text.replace(re, `<u>$&</u>`);
    }

    onClosed: {
        searchField.text = "";
    }
}
