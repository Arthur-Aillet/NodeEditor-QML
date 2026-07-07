pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Menu {
    id: nodePortSearchMenu
    width: 250

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    required property NavigableArea area
    property int portSide
    property int portIndex
    property int nodeIndex
    property nodeDataType portDataType
    property real openedAtX
    property real openedAtY

    popupType: Popup.Window
    onOpened: {
        instanciator.active = false;
        searchField.forceActiveFocus(Qt.PopupFocusReason);
        instanciator.active = true;
    }
    onClosed: {
        portSearchModel.clear();
        instanciator.active = false;
        list.currentIndex = -1;
        searchField.text = "";
    }

    NodeSearchTextField {
        id: searchField
        listView: list
        sfpModel: sfpm
        onAccepted: {
            let name;
            let portInfo;
            if (list.currentIndex == -1) {
                if (list.itemAtIndex(0) == null)
                    return;
                name = (list.itemAtIndex(0) as NodePortSearchMenuItem).name;
                portInfo = (list.itemAtIndex(0) as NodePortSearchMenuItem).port;
            } else {
                name = (list.currentItem as NodePortSearchMenuItem).name;
                portInfo = (list.currentItem as NodePortSearchMenuItem).port;
            }
            const pos = Qt.point(nodePortSearchMenu.x, nodePortSearchMenu.y);
            const newNodeId = ModelInterface.createNode(name, nodePortSearchMenu.area.mapToItem(nodePortSearchMenu.area.inner, pos));
            let newConnection;
            if (nodePortSearchMenu.portSide === NodeEditor.PortType.In) {
                newConnection = {
                    inNodeId: nodePortSearchMenu.nodeIndex,
                    inPortIndex: nodePortSearchMenu.portIndex,
                    outNodeId: newNodeId,
                    outPortIndex: portInfo.portIndex
                };
            } else {
                newConnection = {
                    inNodeId: newNodeId,
                    inPortIndex: portInfo.portIndex,
                    outNodeId: nodePortSearchMenu.nodeIndex,
                    outPortIndex: nodePortSearchMenu.portIndex
                };
            }
            ModelInterface.createConnection(newConnection);
            nodePortSearchMenu.close();
        }
    }

    readonly property var replaceRegex: RegExp(searchField.text.replace(/[\\\.\+\*\?\^\$\[\]\(\)\{\}\/\'\#\:\!\=\|]/ig, "\\$&"), 'gi')

    MenuSeparator {
        id: separator
    }

    Instantiator {
        id: instanciator
        model: DataFlowModelInterface.dataFlowGraph.registry.nodesModel
        delegate: NodeRoleData {
            required property string name
            required property string category
            required property list<portInfo> portsInfo
        }
        onObjectAdded: (index, object) => {
            let data = object as NodeRoleData;
            for (let i = 0; i != data.portsInfo.length; i++) {
                if (data.portsInfo[i].portType == nodePortSearchMenu.portSide) {
                    continue;
                }
                if (nodePortSearchMenu.portSide == NodeEditor.PortType.In) {
                    if (!data.portsInfo[i].dataType.compatibleTypes.includes(nodePortSearchMenu.portDataType.id)) {
                        continue;
                    }
                }
                if (nodePortSearchMenu.portSide == NodeEditor.PortType.Out) {
                    if (!nodePortSearchMenu.portDataType.compatibleTypes.includes(data.portsInfo[i].dataType.id)) {
                        continue;
                    }
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
                const pos = Qt.point(nodePortSearchMenu.openedAtX, nodePortSearchMenu.openedAtY);
                const newNodeId = ModelInterface.createNode(name, nodePortSearchMenu.area.mapToItem(nodePortSearchMenu.area.inner, pos));
                let newConnection;
                if (nodePortSearchMenu.portSide === NodeEditor.PortType.In) {
                    newConnection = {
                        inNodeId: nodePortSearchMenu.nodeIndex,
                        inPortIndex: nodePortSearchMenu.portIndex,
                        outNodeId: newNodeId,
                        outPortIndex: port.portIndex
                    };
                } else {
                    newConnection = {
                        inNodeId: newNodeId,
                        inPortIndex: port.portIndex,
                        outNodeId: nodePortSearchMenu.nodeIndex,
                        outPortIndex: nodePortSearchMenu.portIndex
                    };
                }
                ModelInterface.createConnection(newConnection);
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
