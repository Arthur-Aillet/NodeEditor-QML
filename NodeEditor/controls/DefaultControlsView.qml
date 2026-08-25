import QtQuick
import QtQuick.Controls

import NodeEditor

GraphicsView {
    id: controlledView

    required property DataFlowContext dataFlowContext
    context: dataFlowContext as AbstractContext

    function mapPos(pos: point): point {
        return area.inner.mapToItem(area, pos);
    }

    Connections {
        target: controlledView.context.graphModel
        function onConnectionDropped(nodeId: int, portSide: int, portId: int, x: int, y: int) {
            portSearch.portSide = portSide;
            portSearch.portIndex = portId;
            portSearch.nodeIndex = nodeId;
            portSearch.portDataType = controlledView.context.graphModel.portData(nodeId, portSide, portId, NodeEditor.PortRole.DataType);
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
        context: controlledView.dataFlowContext
    }

    NodeSearchMenu {
        id: search
        area: controlledView.area
        context: controlledView.dataFlowContext
    }

    PortSearchMenu {
        id: portSearch
        area: controlledView.area
        graphicsView: controlledView
        context: controlledView.dataFlowContext
    }
}
