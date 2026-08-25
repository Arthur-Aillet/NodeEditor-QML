import QtQuick
import NodeEditor

Item {
    id: connectionObject
    required property NodeList nodes
    required property NavigableArea area
    required property AbstractContext context

    required property connectionId connection

    property bool hovered: false

    function distanceToCurve(point: point): real {
        return (loader.item as AbstractConnectionPainter).distanceToCurve(point);
    }

    x: !loader.item ? 0 : (loader.item as AbstractConnectionPainter).boundingBox.x
    y: !loader.item ? 0 : (loader.item as AbstractConnectionPainter).boundingBox.y
    width: !loader.item ? 0 : (loader.item as AbstractConnectionPainter).boundingBox.width
    height: !loader.item ? 0 : (loader.item as AbstractConnectionPainter).boundingBox.height

    function getPortPosition(side): var {
        let nodeId;
        let portIndex;

        if (side == NodeEditor.PortSide.In) {
            nodeId = connection.inNodeId;
            portIndex = connection.inPortIndex;
        } else {
            nodeId = connection.outNodeId;
            portIndex = connection.outPortIndex;
        }

        for (let i = 0; i != nodes.nodes.count; i++) {
            const node = nodes.nodes.itemAt(i) as NodeObject;

            if (node.nodeId == nodeId) {
                const portPos = node.getPortPosition(portIndex, side);

                return Qt.point(node.x + portPos.x, node.y + portPos.y);
            }
        }
    }

    property point inPoint: {
        if (connection.inNodeId === NodeEditorUtils.InvalidNodeId)
            return area.mousePosition;
        return getPortPosition(NodeEditor.PortSide.In);
    }
    property point outPoint: {
        if (connection.outNodeId === NodeEditorUtils.InvalidNodeId)
            return area.mousePosition;
        return getPortPosition(NodeEditor.PortSide.Out);
    }

    property bool fullyConnected: connection.inNodeId !== NodeEditorUtils.InvalidNodeId && connection.outNodeId !== NodeEditorUtils.InvalidNodeId
    focusPolicy: hovered ? Qt.ClickFocus : Qt.NoFocus

    TapHandler {
        enabled: connectionObject.hovered
        onTapped: connectionObject.focus = true
    }

    Keys.onDeletePressed: {
        if (fullyConnected)
            context.graphModel.deleteConnection(connection);
    }

    Keys.onBackPressed: {
        if (fullyConnected)
            context.graphModel.deleteConnection(connection);
    }

    Loader {
        id: loader
        Component.onCompleted: {
            setSource(connectionObject.context.connectionPainter.url, {
                obj: connectionObject
            });
        }
    }
}
