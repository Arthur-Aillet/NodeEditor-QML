pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Menu {
    id: nodeListMenu
    popupType: Popup.Window

    required property DataFlowContext context
    required property NavigableArea area
    required property NodeSearchMenu searchMenu
    property point openedAt

    cascade: true

    function openAt(pos: point) {
        x = pos.x;
        y = pos.y;
        openedAt = pos;
        open();
    }

    FontLoader {
        id: iconFont
        source: "/NodeEditor/icomoon.ttf"
    }

    Button {
        id: searchButton
        contentItem: Text {
            text: " \uE986 Search..."
            color: palette.text
            font.family: iconFont.name
            horizontalAlignment: Text.AlignLeft
        }
        onClicked: {
            nodeListMenu.searchMenu.openAt(nodeListMenu.openedAt);
            nodeListMenu.close();
        }
    }
    MenuSeparator {}

    Instantiator {
        model: nodeListMenu.context.dataFlowGraphModel.registry.categories
        delegate: Menu {
            id: categoryMenu
            required property string modelData
            title: modelData

            SortFilterProxyModel {
                id: sfpm
                model: nodeListMenu.context.dataFlowGraphModel.registry.nodesModel
                sorters: [
                    RoleSorter {
                        roleName: "name"
                        priority: 0
                    }
                ]
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

                    onTriggered: {
                        const pos = Qt.point(nodeListMenu.x, nodeListMenu.y);
                        const nodeId = nodeListMenu.context.graphModel.addNode(name);
                        if (nodeId != NodeEditorUtils.InvalidNodeId) {
                            nodeListMenu.context.graphModel.setNodeData(nodeId, NodeEditor.NodeRole.Position, nodeListMenu.area.mapToItem(nodeListMenu.area.inner, pos));
                        }
                        nodeListMenu.close();
                    }
                }
                onObjectAdded: (index, object) => categoryMenu.insertItem(index, object)
                onObjectRemoved: (index, object) => categoryMenu.removeItem(object)
            }
        }
        onObjectAdded: (index, object) => nodeListMenu.insertMenu(index + 2, object)
        onObjectRemoved: (index, object) => nodeListMenu.removeMenu(object)
    }
    component RoleData: QtObject {
        property string name
        property string category
        property var portsInfo
    }
}
