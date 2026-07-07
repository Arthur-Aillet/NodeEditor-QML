import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import CutieDesigner.App.Menu

import NodeEditor

MouseArea {
    id: nodeView
    property alias graphicsView: graphicsView

    hoverEnabled: true
    propagateComposedEvents: true

    Layout.fillWidth: true
    Layout.fillHeight: true

    Keys.onPressed: event => {
        if (event.key == Qt.Key_N) {
            search.x = mouseX;
            search.y = mouseY;
            search.openedAtX = mouseY;
            search.openedAtY = mouseY;
            search.open();
        }
    }

    Connections {
        target: ModelInterface.graph
        function onConnectionDropped(nodeId: int, portType: int, portId: int, x: int, y: int) {
            portSearch.portSide = portType;
            portSearch.portIndex = portId;
            portSearch.nodeIndex = nodeId;
            portSearch.portDataType = ModelInterface.graph.portData(nodeId, portType, portId, NodeEditor.PortRole.DataType);
            const mappedPos = graphicsView.area.inner.mapToItem(graphicsView.area, Qt.point(x, y));
            portSearch.x = mappedPos.x;
            portSearch.y = mappedPos.y;
            portSearch.openedAtX = mappedPos.x;
            portSearch.openedAtY = mappedPos.y;
            portSearch.open();
        }
    }

    GraphicsView {
        id: graphicsView
        height: parent.height - timeline.height
        anchors.left: parent.left
        anchors.right: parent.right

        ContextMenu.onRequested: position => {
            listMenu.x = position.x;
            listMenu.y = position.y;
            listMenu.openedAtX = position.x;
            listMenu.openedAtY = position.y;
            listMenu.open();
        }
        NodeListMenu {
            id: listMenu
            area: graphicsView.area
            searchMenu: search
        }
        NodeSearchMenu {
            id: search
            area: graphicsView.area
        }
        NodePortSearchMenu {
            id: portSearch
            area: graphicsView.area
        }
    }

    Timeline {
        id: timeline
        anchors.top: graphicsView.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
