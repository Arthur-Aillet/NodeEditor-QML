pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Menu {
    id: nodeSearchMenu
    width: 250

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    required property NavigableArea area

    popupType: Popup.Window
    onOpened: {
        searchField.forceActiveFocus(Qt.PopupFocusReason);
    }
    onClosed: {
        list.currentIndex = -1;
        searchField.text = "";
    }

    TextField {
        id: searchField
        placeholderText: "Filter"
        onTextChanged: {
            list.currentIndex = -1;
            sfpm.invalidate();
        }
        hoverEnabled: true
        onHoveredChanged: {
            if (hovered) {
                forceActiveFocus(Qt.PopupFocusReason);
            }
        }
        onAccepted: {
            let name;
            if (list.currentIndex == -1) {
                if (list.itemAtIndex(0) == null)
                    return;
                name = list.itemAtIndex(0).name;
            } else {
                name = list.currentItem.name;
            }
            const pos = Qt.point(nodeSearchMenu.x, nodeSearchMenu.y);
            ModelInterface.createNode(name, nodeSearchMenu.area.mapToItem(nodeSearchMenu.area.inner, pos));
            nodeSearchMenu.close();
        }
        Keys.onDownPressed: {
            list.incrementCurrentIndex();
        }
        Keys.onUpPressed: {
            list.decrementCurrentIndex();
        }
        Keys.onTabPressed: {
            list.incrementCurrentIndex();
        }
        Keys.onBacktabPressed: {
            list.decrementCurrentIndex();
        }
    }
    SortFilterProxyModel {
        id: sfpm
        model: DataFlowModelInterface.dataFlowGraph.registry.model
        sorters: [
            RoleSorter {
                roleName: "category"
                priority: 0
            },
            RoleSorter {
                roleName: "name"
                priority: 1
            }
        ]
        filters: [
            FunctionFilter {
                function filter(data: RoleData): bool {
                    if (searchField.text == "")
                        return true;
                    return data["name"].toLowerCase().includes(searchField.text.toLowerCase());
                }
            }
        ]
    }

    readonly property var replaceRegex: RegExp(searchField.text.replace(/[\\\.\+\*\?\^\$\[\]\(\)\{\}\/\'\#\:\!\=\|]/ig, "\\$&"), 'gi')

    MenuSeparator {
        id: separator
    }
    ListView {
        id: list
        clip: true
        width: parent.width
        anchors.top: separator.bottom
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 200
        focusPolicy: Qt.WheelFocus
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            active: ScrollBar.AlwaysOn
        }
        interactive: true
        model: sfpm

        Keys.forwardTo: [searchField]

        delegate: MouseArea {
            id: nodeModel
            required property string name
            required property string category
            required property int index
            height: 25
            width: list.width
            propagateComposedEvents: true
            hoverEnabled: true
            onEntered: {
                list.currentIndex = index;
            }
            onPressed: {
                const pos = Qt.point(nodeSearchMenu.x, nodeSearchMenu.y);
                ModelInterface.createNode(name, nodeSearchMenu.area.mapToItem(nodeSearchMenu.area.inner, pos));
                nodeSearchMenu.close();
            }
            Rectangle {
                anchors.fill: parent
                color: list.currentIndex == nodeModel.index ? palette.highlight : ((nodeModel.index % 2 !== 0) ? palette.base : palette.alternateBase)
            }
            Text {
                id: category
                text: " " + nodeModel.category + "  🞂  "
                color: palette.placeholderText
                opacity: list.currentIndex == nodeModel.index ? 1 : 0.8
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                function computeName(): string {
                    return nodeModel.name.replace(nodeSearchMenu.replaceRegex, `<u>$&</u>`);
                }

                text: computeName()
                color: palette.text
                opacity: list.currentIndex == nodeModel.index ? 1 : 0.8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: category.right
            }
        }
        // Rectangle {
        //     x: {
        //         if (!delegate.hasChildren)
        //             return 0;
        //         const indent = delegate.isTreeNode ? (delegate.depth + 1) * delegate.indentation : 0;
        //         return delegate.padding + indent + 4;
        //     }
        //     width: parent.width - delegate.padding - x
        //     height: parent.height
        //     color: Qt.rgba(palette.highlight.r, palette.highlight.g, palette.highlight.b, delegate.hasChildren ? 0.2 : 1)
        //     radius: delegate.hasChildren ? 4 : 0
        //     border.color: palette.highlight
        //     border.width: delegate.hasChildren ? 1 : 0

        //     opacity: delegate.row === view.currentRow ? 1 : 0
        // }

        // Button {
        //     required property string name
        //     required property string category

        //     text: category + " > " + name
        // }
    }
    component RoleData: QtObject {
        property string name
        property string category
        property var portsInfo
    }
}
