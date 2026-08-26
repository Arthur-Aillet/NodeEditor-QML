pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Menu {
    id: portSearchMenu
    width: 250

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    required property DataFlowContext context
    required property NavigableArea area
    required property GraphicsView graphicsView
    property int portSide
    property int portIndex
    property int nodeIndex
    property nodeDataType portDataType
    property point openedAt

    function openAt(pos: point) {
        x = pos.x;
        y = pos.y;
        openedAt = pos;
        open();
    }

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
        onAccepted: {
            let name;
            let portInfo;
            if (list.currentIndex == -1) {
                if (list.itemAtIndex(0) == null)
                    return;
                name = (list.itemAtIndex(0) as SearchMenuItem).name;
                portInfo = (list.itemAtIndex(0) as SearchMenuItem).port;
            } else {
                name = (list.currentItem as SearchMenuItem).name;
                portInfo = (list.currentItem as SearchMenuItem).port;
            }
            const pos = Qt.point(portSearchMenu.x, portSearchMenu.y);
            const newNodeId = portSearchMenu.context.graphModel.addNode(name);
            if (newNodeId != NodeEditorUtils.InvalidNodeId) {
                const mappedPos = portSearchMenu.area.mapToItem(portSearchMenu.area.inner, pos);
                portSearchMenu.context.graphModel.setNodeData(newNodeId, NodeEditor.NodeRole.Position, mappedPos);
            }
            let newConnection;
            if (portSearchMenu.portSide === NodeEditor.PortSide.In) {
                newConnection = {
                    inNodeId: portSearchMenu.nodeIndex,
                    inPortIndex: portSearchMenu.portIndex,
                    outNodeId: newNodeId,
                    outPortIndex: portInfo.portIndex
                };
            } else {
                newConnection = {
                    inNodeId: newNodeId,
                    inPortIndex: portInfo.portIndex,
                    outNodeId: portSearchMenu.nodeIndex,
                    outPortIndex: portSearchMenu.portIndex
                };
            }
            portSearchMenu.context.graphModel.addConnection(newConnection);
            portSearchMenu.close();
        }
    }

    readonly property var replaceRegex: RegExp(searchField.text.replace(/[\\\.\+\*\?\^\$\[\]\(\)\{\}\/\'\#\:\!\=\|]/ig, "\\$&"), 'gi')

    MenuSeparator {
        id: separator
    }

    Instantiator {
        id: instanciator
        model: portSearchMenu.context.dataFlowGraphModel.registry.nodesModel
        delegate: NodeRoleData {
            required property string name
            required property string category
            required property list<portInfo> portsInfo
        }
        onObjectAdded: (index, object) => {
            let data = object as NodeRoleData;
            for (let i = 0; i != data.portsInfo.length; i++) {
                if (data.portsInfo[i].portSide == portSearchMenu.portSide) {
                    continue;
                }
                if (portSearchMenu.portSide == NodeEditor.PortSide.In) {
                    if (!data.portsInfo[i].dataType.compatibleTypes.includes(portSearchMenu.portDataType.id)) {
                        continue;
                    }
                }
                if (portSearchMenu.portSide == NodeEditor.PortSide.Out) {
                    if (!portSearchMenu.portDataType.compatibleTypes.includes(data.portsInfo[i].dataType.id)) {
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

    SearchFilterModel {
        id: smf
        filterText: searchField.text
        model: portSearchModel
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
        model: smf

        Keys.forwardTo: [searchField]

        delegate: SearchMenuItem {
            currentIndex: list.currentIndex
            replaceRegex: portSearchMenu.replaceRegex
            width: list.width
            required property portInfo portInfo
            port: portInfo
            typeColor: portSearchMenu.context.styles.connection.typeColor(portInfo.dataType.id)

            onEntered: {
                list.currentIndex = index;
            }
            onPressed: {
                const newNodeId = portSearchMenu.context.graphModel.addNode(name);
                let newConnection;
                if (portSearchMenu.portSide === NodeEditor.PortSide.In) {
                    newConnection = {
                        inNodeId: portSearchMenu.nodeIndex,
                        inPortIndex: portSearchMenu.portIndex,
                        outNodeId: newNodeId,
                        outPortIndex: port.portIndex
                    };
                } else {
                    newConnection = {
                        inNodeId: newNodeId,
                        inPortIndex: port.portIndex,
                        outNodeId: portSearchMenu.nodeIndex,
                        outPortIndex: portSearchMenu.portIndex
                    };
                }
                portSearchMenu.context.graphModel.addConnection(newConnection);
                let mappedPos = portSearchMenu.area.mapToItem(portSearchMenu.area.inner, portSearchMenu.openedAt);
                const newNode = portSearchMenu.graphicsView.nodes.nodeAt(newNodeId);
                const portPos = newNode.getPortPosition(port.portIndex, NodeEditorUtils.oppositeSide(portSearchMenu.portSide));
                mappedPos.x -= portPos.x;
                mappedPos.y -= portPos.y;
                portSearchMenu.context.graphModel.setNodeData(newNodeId, NodeEditor.NodeRole.Position, mappedPos);
                portSearchMenu.close();
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
