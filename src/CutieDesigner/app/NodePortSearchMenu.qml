pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Menu {
    id: nodePortSearchMenu
    width: 250

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    required property NavigableArea area
    property int portType: NodeEditor.PortType.In
    property string portInfoType: "decimal"

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
            const pos = Qt.point(nodePortSearchMenu.x, nodePortSearchMenu.y);
            ModelInterface.createNode(name, nodePortSearchMenu.area.mapToItem(nodePortSearchMenu.area.inner, pos));
            nodePortSearchMenu.close();
        }
    }

    readonly property var replaceRegex: RegExp(searchField.text.replace(/[\\\.\+\*\?\^\$\[\]\(\)\{\}\/\'\#\:\!\=\|]/ig, "\\$&"), 'gi')

    MenuSeparator {
        id: separator
    }

    Instantiator {
        model: DataFlowModelInterface.dataFlowGraph.registry.nodesModel
        delegate: NodeRoleData {
            required property string name
            required property string category
            required property list<portInfo> portsInfo
        }
        onObjectAdded: (index, object) => {
            let data = object as NodeRoleData;
            for (let i = 0; i != data.portsInfo.length; i++) {
                if (data.portsInfo[i].portType == nodePortSearchMenu.portType) {
                    continue;
                }
                if (!data.portsInfo[i].dataType.compatibleTypes.includes(nodePortSearchMenu.portInfoType)) {
                    continue;
                }

                portSearchModel.append({
                    "name": data.name,
                    "category": data.category,
                    "portInfo": data.portsInfo[i]
                });
            }
        }
        onObjectRemoved: (index, object) => {
            for (let i = portSearchModel.count; i != 0; i--) {
                let data = object as PortRoleData;
                if (portSearchModel.get(i).name == data.name && portSearchModel.get(i).category == data.category) {
                    portSearchModel.remove(i);
                }
            }
        }
    }

    ListModel {
        id: portSearchModel
    }

    SortFilterProxyModel {
        id: sfpm
        model: portSearchModel
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
                function filter(data: PortRoleData): bool {
                    if (searchField.text == "")
                        return true;
                    return data["name"].toLowerCase().includes(searchField.text.toLowerCase());
                }
            }
        ]
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
            replaceRegex: nodePortSearchMenu.replaceRegex
            width: list.width
            required property portInfo portInfo
            port: portInfo

            onEntered: {
                list.currentIndex = index;
            }
            onPressed: {
                const pos = Qt.point(nodePortSearchMenu.x, nodePortSearchMenu.y);
                ModelInterface.createNode(name, nodePortSearchMenu.area.mapToItem(nodePortSearchMenu.area.inner, pos));
                nodePortSearchMenu.close();
            }
        }
    }
    component NodeRoleData: QtObject {
        property string name
        property string category
        property list<portInfo> portsInfo
    }
    component PortRoleData: QtObject {
        property string name
        property string category
        property portInfo portInfo
    }
}
