import QtQuick
import QtQuick.Controls

import NodeEditor

GraphicsView {
    id: controlledView

    function mapPos(x: int, y: int): point {
        return area.inner.mapToItem(area, Qt.point(x, y));
    }

    Connections {
        target: ModelInterface.graph
        function onConnectionDropped(nodeId: int, portType: int, portId: int, x: int, y: int) {
            portSearch.portSide = portType;
            portSearch.portIndex = portId;
            portSearch.nodeIndex = nodeId;
            portSearch.portDataType = ModelInterface.graph.portData(nodeId, portType, portId, NodeEditor.PortRole.DataType);
            const mappedPos = controlledView.mapPos(x, y);
            portSearch.x = mappedPos.x;
            portSearch.y = mappedPos.y;
            portSearch.openedAtX = mappedPos.x;
            portSearch.openedAtY = mappedPos.y;
            portSearch.open();
        }
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_N) {
            const mappedPos = mapPos(area.mousePosition.x, area.mousePosition.y);
            search.x = mappedPos.x;
            search.y = mappedPos.y;
            search.openedAtX = mappedPos.x;
            search.openedAtY = mappedPos.y;
            search.open();
        }
    }

    ContextMenu.onRequested: position => {
        listMenu.x = position.x;
        listMenu.y = position.y;
        listMenu.openedAtX = position.x;
        listMenu.openedAtY = position.y;
        listMenu.open();
    }

    NodeListMenu {
        id: listMenu
        area: controlledView.area
        searchMenu: search
    }

    NodeSearchMenu {
        id: search
        area: controlledView.area
    }

    PortSearchMenu {
        id: portSearch
        area: controlledView.area
        graphicsView: controlledView
    }
}
