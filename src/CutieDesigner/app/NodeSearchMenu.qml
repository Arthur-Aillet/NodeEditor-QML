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

    NodeSearchTextField {
        id: searchField
        listView: list
        sfpModel: sfpm
        onAccepted: {
            let name;
            if (list.currentIndex == -1) {
                if (list.itemAtIndex(0) == null)
                    return;
                name = (list.itemAtIndex(0) as NodeSearchMenuItem).name;
            } else {
                name = (list.currentItem as NodeSearchMenuItem).name;
            }
            const pos = Qt.point(nodeSearchMenu.x, nodeSearchMenu.y);
            ModelInterface.createNode(name, nodeSearchMenu.area.mapToItem(nodeSearchMenu.area.inner, pos));
            nodeSearchMenu.close();
        }
    }

    SortFilterProxyModel {
        id: sfpm
        model: DataFlowModelInterface.dataFlowGraph.registry.nodesModel
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

        delegate: NodePortSearchMenuItem {
            currentIndex: list.currentIndex
            replaceRegex: nodeSearchMenu.replaceRegex
            width: list.width

            onEntered: {
                list.currentIndex = index;
            }
            onPressed: {
                const pos = Qt.point(nodeSearchMenu.x, nodeSearchMenu.y);
                ModelInterface.createNode(name, nodeSearchMenu.area.mapToItem(nodeSearchMenu.area.inner, pos));
                nodeSearchMenu.close();
            }
        }
    }
    component RoleData: QtObject {
        property string name
        property string category
        property list<portInfo> portsInfo
    }
}
