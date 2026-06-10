import QtQuick
import NodeEditor

Canvas {
    id: root

    required property NodeList nodes
    required property NavigableArea area

    Connections {
        target: ModelInterface

        function onNodePositionUpdated(nodeId: real) {
            if (nodeId == root.connection.inNodeId) {
                root.inNodePos = ModelInterface.graph.nodeData(nodeId, NodeEditor.NodeRole.Position);
            }
            if (nodeId == root.connection.outNodeId) {
                root.outNodePos = ModelInterface.graph.nodeData(nodeId, NodeEditor.NodeRole.Position);
            }
        }
    }

    required property connectionId connection

    function getPortPosition(side): var {
        let nodeId;
        let portIndex;

        if (side == NodeEditor.PortType.In) {
            nodeId = connection.inNodeId;
            portIndex = connection.inPortIndex;
        } else {
            nodeId = connection.outNodeId;
            portIndex = connection.outPortIndex;
        }

        for (let i = 0; i != nodes.nodes.count; i++) {
            const node = nodes.nodes.itemAt(i) as NodeGraphicalObject;

            if (node.nodeId == nodeId) {
                const portPos = node.getPortPosition(portIndex, side);

                return Qt.point(node.x + portPos.x, node.y + portPos.y);
            }
        }
    }

    property point inPoint: {
        if (connection.inNodeId === NodeEditorUtils.InvalidNodeId)
            return area.mousePosition;
        return getPortPosition(NodeEditor.PortType.In);
    }
    property point outPoint: {
        if (connection.outNodeId === NodeEditorUtils.InvalidNodeId)
            return area.mousePosition;
        return getPortPosition(NodeEditor.PortType.Out);
    }

    property color inTypeColor: {
        if (!fullyConnected)
            return "black";
        const dataType = ModelInterface.graph.portData(connection.inNodeId, NodeEditor.PortType.In, connection.inPortIndex, NodeEditor.PortRole.DataType);
        return StyleCollection.connection.typeColor(dataType);
    }

    property color outTypeColor: {
        if (!fullyConnected)
            return "black";
        const dataType = ModelInterface.graph.portData(connection.outNodeId, NodeEditor.PortType.Out, connection.outPortIndex, NodeEditor.PortRole.DataType);
        return StyleCollection.connection.typeColor(dataType);
    }

    property point c1
    property point c2

    readonly property real defaultOffset: 200

    property bool horizontal: true
    function refreshPoints() {
        if (horizontal) {
            pointsC1C2Horizontal();
        } else {
            pointsC1C2Vertical();
        }
        markDirty(Qt.rect(x, y, width, height));
    }
    onInPointChanged: refreshPoints()
    onOutPointChanged: refreshPoints()

    function pointsC1C2Horizontal() {
        const xDistance = inPoint.x - outPoint.x;
        let horizontalOffset = Math.min(defaultOffset, Math.abs(xDistance));
        let verticalOffset = 0;
        let ratioX = 0.5;
        if (xDistance <= 0) {
            let yDistance = inPoint.y - outPoint.y + 20;
            let vector = yDistance < 0 ? -1.0 : 1.0;
            verticalOffset = Math.min(defaultOffset, Math.abs(yDistance)) * vector;
            ratioX = 1.0;
        }
        horizontalOffset *= ratioX;
        c1 = Qt.point(inPoint.x - horizontalOffset, inPoint.y - verticalOffset);
        c2 = Qt.point(outPoint.x + horizontalOffset, outPoint.y + verticalOffset);
    }

    function pointsC1C2Vertical() {
        const yDistance = inPoint.y - outPoint.y;
        let verticalOffset = Math.min(defaultOffset, Math.abs(yDistance));
        let horizontalOffset = 0;
        let ratioY = 0.5;
        if (yDistance <= 0) {
            let xDistance = inPoint.x - outPoint.x + 20;
            let vector = xDistance < 0 ? -1.0 : 1.0;
            horizontalOffset = Math.min(defaultOffset, Math.abs(xDistance)) * vector;
            ratioY = 1.0;
        }
        verticalOffset *= ratioY;
        c1 = Qt.point(inPoint.x - horizontalOffset, inPoint.y - verticalOffset);
        c2 = Qt.point(outPoint.x + horizontalOffset, outPoint.y + verticalOffset);
    }

    property bool fullyConnected: connection.inNodeId !== NodeEditorUtils.InvalidNodeId && connection.outNodeId !== NodeEditorUtils.InvalidNodeId
    property bool hovered: false

    onHoveredChanged: markDirty(Qt.rect(x, y, width, height))
    onFocusChanged: markDirty(Qt.rect(x, y, width, height))

    function distanceToCurve(point: point): real {
        const overlapAndCurveFactor = 1.2;
        const linearLength = Math.sqrt(Math.pow(inPoint.x - outPoint.x, 2) + Math.pow(inPoint.y - outPoint.y, 2));
        const steps = Math.round(linearLength / 10 * overlapAndCurveFactor);
        let shortestDistance = Infinity;
        for (let i = 0; i != steps; i++) {
            const pointAlong = path.pointAtPercent(1 / steps * i);
            const diff = Qt.vector2d(pointAlong.x - point.x, pointAlong.y - point.y);
            const dist = Math.sqrt(diff.dotProduct(diff));
            if (dist < shortestDistance)
                shortestDistance = dist;
        }
        return shortestDistance;
    }

    TapHandler {
        enabled: root.hovered
        onTapped: root.focus = true
    }

    Keys.onPressed: event => {
        if (!fullyConnected)
            return;

        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            ModelInterface.deleteConnection(connection);
        }
    }

    property bool swap: connection.inNodeId === NodeEditorUtils.InvalidNodeId

    property double scaleFactor: root.area.inner.mat.m11

    x: (Math.min(root.inPoint.x, root.outPoint.x, root.c1.x, root.c2.x)) - StyleCollection.connection.lineWidth
    y: (Math.min(root.inPoint.y, root.outPoint.y, root.c1.y, root.c2.y)) - StyleCollection.connection.lineWidth
    width: ((Math.max(root.inPoint.x, root.outPoint.x, root.c1.x, root.c2.x) - x) + StyleCollection.connection.lineWidth * 3) * scaleFactor
    height: ((Math.max(root.inPoint.y, root.outPoint.y, root.c1.y, root.c2.y) - y) + StyleCollection.connection.lineWidth * 3) * scaleFactor

    transformOrigin: Item.TopLeft

    scale: 1.0 / scaleFactor

    antialiasing: true

    property point scaledStartPoint: Qt.point((path.startX - x) * scaleFactor, (path.startY - y) * scaleFactor)
    property point scaledEndPoint: Qt.point((pathCubic.x - x) * scaleFactor, (pathCubic.y - y) * scaleFactor)

    function innerStrokeStyle(ctx): color {
        if (focus)
            return StyleCollection.connection.selectedColor;
        if (!StyleCollection.connection.useDataDefinedColors)
            return StyleCollection.connection.normalColor;
        if (root.inTypeColor == root.outTypeColor)
            return root.inTypeColor;

        const gradient = ctx.createLinearGradient(scaledStartPoint.x, scaledStartPoint.y, scaledEndPoint.x, scaledEndPoint.y);

        for (let i = 0; i != 10; i++) {
            const color = StyleCollection.connection.lerpOklabColors(root.inTypeColor, root.outTypeColor, i / 10);
            gradient.addColorStop(i / 10, color);
        }

        return gradient;
    }
    onPaint: {
        let ctx = getContext('2d');
        ctx.clearRect(0, 0, width, height);

        const c1 = Qt.point((pathCubic.control1X - x) * scaleFactor, (pathCubic.control1Y - y) * scaleFactor);
        const c2 = Qt.point((pathCubic.control2X - x) * scaleFactor, (pathCubic.control2Y - y) * scaleFactor);

        ctx.beginPath();
        ctx.lineCap = "round";
        ctx.moveTo(scaledStartPoint.x, scaledStartPoint.y);
        ctx.bezierCurveTo(c1.x, c1.y, c2.x, c2.y, scaledEndPoint.x, scaledEndPoint.y);

        if (fullyConnected) {
            ctx.setLineDash([]);
            ctx.lineWidth = StyleCollection.connection.lineWidth * 1.7 * scaleFactor;
            if (focus) {
                ctx.strokeStyle = StyleCollection.connection.selectedHaloColor;
                ctx.stroke();
            } else if (hovered) {
                ctx.strokeStyle = StyleCollection.connection.hoveredColor;
                ctx.stroke();
            }
            ctx.strokeStyle = innerStrokeStyle(ctx);
            ctx.lineWidth = StyleCollection.connection.lineWidth * scaleFactor;
            ctx.stroke();
        } else {
            ctx.setLineDash([6, 2]);
            ctx.lineWidth = StyleCollection.connection.constructionLineWidth * scaleFactor;
            ctx.strokeStyle = StyleCollection.connection.constructionColor;
            ctx.stroke();
        }
    }

    Path {
        id: path

        startX: root.swap ? root.outPoint.x : root.inPoint.x
        startY: root.swap ? root.outPoint.y : root.inPoint.y
        PathCubic {
            id: pathCubic
            x: root.swap ? root.inPoint.x : root.outPoint.x
            y: root.swap ? root.inPoint.y : root.outPoint.y
            control1X: root.swap ? root.c2.x : root.c1.x
            control1Y: root.swap ? root.c2.y : root.c1.y
            control2X: root.swap ? root.c1.x : root.c2.x
            control2Y: root.swap ? root.c1.y : root.c2.y
        }
    }
}
