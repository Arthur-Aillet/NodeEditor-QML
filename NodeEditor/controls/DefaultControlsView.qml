import QtQuick
import QtQuick.Controls

import NodeEditor

GraphicsView {
    id: controlledView

    function mapPos(pos: point): point {
        return area.inner.mapToItem(area, pos);
    }

    Connections {
        target: ModelInterface.graph
        function onConnectionDropped(nodeId: int, portType: int, portId: int, x: int, y: int) {
            portSearch.portSide = portType;
            portSearch.portIndex = portId;
            portSearch.nodeIndex = nodeId;
            portSearch.portDataType = ModelInterface.graph.portData(nodeId, portType, portId, NodeEditor.PortRole.DataType);
            portSearch.openAt(controlledView.mapPos(Qt.point(x, y)));
        }
    }

    Keys.onPressed: event => {
        if (event.key == Qt.Key_N) {
            const mappedPos = mapPos(area.mousePosition.x, area.mousePosition.y);
            search.openAt(mappedPos);
        }
    }

    ContextMenu.onRequested: position => {
        listMenu.openAt(position);
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
