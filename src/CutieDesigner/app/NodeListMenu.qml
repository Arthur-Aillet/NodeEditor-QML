pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Menu {
    id: nodeListMenu
    popupType: Popup.Window
    focus: true

    required property NavigableArea area

    cascade: true

    Instantiator {
        model: DataFlowModelInterface.dataFlowGraph.registry.categories
        delegate: Menu {
            id: categoryMenu
            cascade: true
            required property string modelData
            title: modelData

            SortFilterProxyModel {
                id: sfpm
                model: DataFlowModelInterface.dataFlowGraph.registry.model
                filters: [
                    FunctionFilter {
                        function filter(data: RoleData): bool {
                            return (data["category"] == categoryMenu.modelData);
                        }
                    }
                ]
            }

            Instantiator {
                model: sfpm
                delegate: MenuItem {
                    required property string name
                    text: name
                    onClicked: {
                        const pos = Qt.point(nodeListMenu.x, nodeListMenu.y);
                        ModelInterface.createNode(name, nodeListMenu.area.mapToItem(nodeListMenu.area.inner, pos));
                        nodeListMenu.close();
                    }
                }
                onObjectAdded: (index, object) => categoryMenu.insertItem(index, object)
                onObjectRemoved: (index, object) => categoryMenu.removeItem(object)
            }
        }
        onObjectAdded: (index, object) => nodeListMenu.insertMenu(index, object)
        onObjectRemoved: (index, object) => nodeListMenu.removeMenu(object)
    }

    component RoleData: QtObject {
        property string name
        property string category
        property var portsInfo
    }

    // ListView {
    //     width: 180
    //     height: 200

    //     model: nodeListMenu.nodeMap
    //     delegate: Text {
    //         required property string name
    //         required property string category
    //         text: name + ": " + category
    //     }
    // }

    // // Replace special characters by escaped characters to prevent them to disturb when we use a regex to insert underline tags
    // readonly property var replaceRegex: RegExp(searchField.text.replace(/[\\\.\+\*\?\^\$\[\]\(\)\{\}\/\'\#\:\!\=\|]/ig, "\\$&"), 'gi')

    // TextField {
    //     id: searchField
    //     placeholderText: qsTr("Filter")
    //     Component.onCompleted: searchField.forceActiveFocus(Qt.PopupFocusReason)
    //     onTextChanged: {
    //         //DataFlowModelInterface.dataFlowGraph.filter = text;
    //         view.expandRecursively();
    //     }
    // }

    // component NodeItem: QtObject {
    //     property string display
    //     property var idx
    // }

    // TreeView {
    //     id: view
    //     ScrollBar.vertical: ScrollBar {
    //         policy: ScrollBar.AlwaysOn
    //         active: ScrollBar.AlwaysOn
    //     }

    //     clip: true
    //     width: parent.width
    //     anchors.top: searchField.bottom
    //     anchors.bottom: parent.bottom
    //     boundsBehavior: Flickable.StopAtBounds
    //     editTriggers: TableView.NoEditTriggers

    //     model: nodeListMenu.nodeMap
    //     keyNavigationEnabled: true
    //     selectionModel: ItemSelectionModel {}
    //     delegate: TreeViewDelegate {
    //         id: delegate
    //         contentItem: Label {
    //             id: label
    //             x: padding + (delegate.isTreeNode ? (delegate.depth + 1) * delegate.indentation : 0)
    //             topPadding: 1
    //             anchors.verticalCenter: parent.verticalCenter
    //             width: parent.width - padding - x
    //             clip: true
    //             color: (!delegate.hasChildren && delegate.row === view.currentRow) ? palette.brightText : palette.text
    //             text: delegate.computeText()
    //         }

    //         function computeText(): string {
    //             if (delegate.model.whatsThis != undefined) {
    //                 console.log(Object.keys(delegate.model.___0));
    //                 console.log(delegate.model.whatsThis[0].caption);
    //             }
    //             if (delegate.model.display == undefined)
    //                 return undefined;
    //             if (delegate.hasChildren) {
    //                 return model.display;
    //             }
    //             return model.display.replace(nodeListMenu.replaceRegex, `<u>$&</u>`);
    //         }

    //         background: Rectangle {
    //             id: background
    //             height: parent.height
    //             width: nodeListMenu.width
    //             color: (view.alternatingRows && delegate.row % 2 !== 0) ? palette.base : palette.alternateBase
    //             Rectangle {
    //                 x: {
    //                     if (!delegate.hasChildren)
    //                         return 0;
    //                     const indent = delegate.isTreeNode ? (delegate.depth + 1) * delegate.indentation : 0;
    //                     return delegate.padding + indent + 4;
    //                 }
    //                 width: parent.width - delegate.padding - x
    //                 height: parent.height
    //                 color: Qt.rgba(palette.highlight.r, palette.highlight.g, palette.highlight.b, delegate.hasChildren ? 0.2 : 1)
    //                 radius: delegate.hasChildren ? 4 : 0
    //                 border.color: palette.highlight
    //                 border.width: delegate.hasChildren ? 1 : 0

    //                 opacity: delegate.row === view.currentRow ? 1 : 0
    //             }
    //         }

    //         focus: row === view.currentRow

    //         function activate() {
    //             if (hasChildren) {
    //                 view.toggleExpanded(row);
    //             } else {
    //                 const name = delegate.model.display.toString();
    //                 const pos = Qt.point(nodeListMenu.x, nodeListMenu.y);

    //                 ModelInterface.createNode(name, nodeListMenu.area.mapToItem(nodeListMenu.area.inner, pos));
    //                 nodeListMenu.close();
    //             }
    //         }
    //         onClicked: activate()
    //         Keys.onEnterPressed: activate()
    //     }
    //     Component.onCompleted: expandRecursively()

    //     // Move view to include newly opened rows
    //     // TODO: Logic not perfect yet, this sometimes break
    //     onExpanded: (row, col) => {
    //         if (row < 0)
    //             return;
    //         let modelIndex = view.modelIndex(Qt.point(0, row));

    //         if (modelIndex.parent.valid)
    //             return;
    //         let nextModelIndex = nodeListMenu.nodeMap.sibling(modelIndex.row + 1, 0, modelIndex);
    //         let nextViewRow = view.rowAtIndex(nextModelIndex);
    //         if (nextViewRow == -1) {
    //             view.contentY = view.contentHeight;
    //         } else {
    //             view.positionViewAtRow(nextViewRow - 1, TreeView.Contain);
    //         }
    //     }
    // }

    // onClosed: searchField.text = ""
}
