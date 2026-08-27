pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Menu {
    id: nodeSearchMenu
    width: 250

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    required property DataFlowContext context
    required property NavigableArea area
    property point openedAt

    function openAt(pos: point) {
        x = pos.x;
        y = pos.y;
        openedAt = pos;
        open();
    }

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
        onAccepted: {
            let name;
            if (list.currentIndex == -1) {
                if (list.itemAtIndex(0) == null)
                    return;
                name = (list.itemAtIndex(0) as SearchMenuItem).name;
            } else {
                name = (list.currentItem as SearchMenuItem).name;
            }
            const nodeId = nodeSearchMenu.context.graphModel.addNode(name);
            if (nodeId != NodeEditorUtils.InvalidNodeId) {
                nodeSearchMenu.context.graphModel.setNodeData(nodeId, NodeEditor.NodeRole.Position, nodeSearchMenu.area.mapToItem(nodeSearchMenu.area.inner, nodeSearchMenu.openedAt));
            }
            nodeSearchMenu.close();
        }
    }

    SearchFilterModel {
        id: smf
        filterText: searchField.text
        model: nodeSearchMenu.context.dataFlowGraph.registry.nodesModel
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
        model: smf

        Keys.forwardTo: [searchField]

        delegate: SearchMenuItem {
            currentIndex: list.currentIndex
            replaceRegex: nodeSearchMenu.replaceRegex
            width: list.width

            onEntered: {
                list.currentIndex = index;
            }
            onPressed: {
                const nodeId = nodeSearchMenu.context.graphModel.addNode(name);
                if (nodeId != NodeEditorUtils.InvalidNodeId) {
                    nodeSearchMenu.context.graphModel.setNodeData(nodeId, NodeEditor.NodeRole.Position, nodeSearchMenu.area.mapToItem(nodeSearchMenu.area.inner, nodeSearchMenu.openedAt));
                }
                nodeSearchMenu.close();
            }
        }
    }
}
