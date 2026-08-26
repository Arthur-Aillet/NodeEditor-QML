import QtQuick
import NodeEditor

AbstractConnectionPainter {
    id: defaultConnectionPainter

    property color inTypeColor: {
        if (!obj.fullyConnected)
            return "black";
        const dataType = obj.context.graphModel.portData(obj.connection.inNodeId, NodeEditor.PortSide.In, obj.connection.inPortIndex, NodeEditor.PortRole.DataType);
        return obj.context.styles.connection.typeColor(dataType.id);
    }

    property color outTypeColor: {
        if (!obj.fullyConnected)
            return "black";
        const dataType = obj.context.graphModel.portData(obj.connection.outNodeId, NodeEditor.PortSide.Out, obj.connection.outPortIndex, NodeEditor.PortRole.DataType);
        return obj.context.styles.connection.typeColor(dataType.id);
    }

    property point c1
    property point c2

    Component.onCompleted: {
        refreshPoints();
    }

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

    function pointsC1C2Horizontal() {
        const xDistance = obj.inPoint.x - obj.outPoint.x;
        let horizontalOffset = Math.min(defaultOffset, Math.abs(xDistance));
        let verticalOffset = 0;
        let ratioX = 0.5;
        if (xDistance <= 0) {
            let yDistance = obj.inPoint.y - obj.outPoint.y + 20;
            let vector = yDistance < 0 ? -1.0 : 1.0;
            verticalOffset = Math.min(defaultOffset, Math.abs(yDistance)) * vector;
            ratioX = 1.0;
        }
        horizontalOffset *= ratioX;
        c1 = Qt.point(obj.inPoint.x - horizontalOffset, obj.inPoint.y - verticalOffset);
        c2 = Qt.point(obj.outPoint.x + horizontalOffset, obj.outPoint.y + verticalOffset);
    }

    function pointsC1C2Vertical() {
        const yDistance = obj.inPoint.y - obj.outPoint.y;
        let verticalOffset = Math.min(defaultOffset, Math.abs(yDistance));
        let horizontalOffset = 0;
        let ratioY = 0.5;
        if (yDistance <= 0) {
            let xDistance = obj.inPoint.x - obj.outPoint.x + 20;
            let vector = xDistance < 0 ? -1.0 : 1.0;
            horizontalOffset = Math.min(defaultOffset, Math.abs(xDistance)) * vector;
            ratioY = 1.0;
        }
        verticalOffset *= ratioY;
        c1 = Qt.point(obj.inPoint.x - horizontalOffset, obj.inPoint.y - verticalOffset);
        c2 = Qt.point(obj.outPoint.x + horizontalOffset, obj.outPoint.y + verticalOffset);
    }

    Connections {
        property alias dc: defaultConnectionPainter

        target: dc.obj
        function onHoveredChanged() {
            dc.markDirty(Qt.rect(dc.x, dc.y, dc.width, dc.height));
        }

        function onActiveFocusChanged() {
            dc.markDirty(Qt.rect(dc.x, dc.y, dc.width, dc.height));
        }

        function onInPointChanged() {
            dc.refreshPoints();
        }

        function onOutPointChanged() {
            dc.refreshPoints();
        }
    }

    override property var distanceToCurve: point => {
        const overlapAndCurveFactor = 1.2;
        const linearLength = Math.sqrt(Math.pow(obj.inPoint.x - obj.outPoint.x, 2) + Math.pow(obj.inPoint.y - obj.outPoint.y, 2));
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

    property double scaleFactor: obj.area.inner.mat.m11

    width: obj.width * scaleFactor
    height: obj.height * scaleFactor

    override property rect boundingBox: {
        const bbx = Math.min(obj.inPoint.x, obj.outPoint.x, c1.x, c2.x) - obj.context.styles.connection.lineWidth;
        const bby = Math.min(obj.inPoint.y, obj.outPoint.y, c1.y, c2.y) - obj.context.styles.connection.lineWidth;

        return {
            x: bbx,
            y: bby,
            width: (Math.max(obj.inPoint.x, obj.outPoint.x, c1.x, c2.x) - bbx) + obj.context.styles.connection.lineWidth * 3,
            height: (Math.max(obj.inPoint.y, obj.outPoint.y, c1.y, c2.y) - bby) + obj.context.styles.connection.lineWidth * 3
        };
    }

    transformOrigin: Item.TopLeft

    scale: 1.0 / scaleFactor

    antialiasing: true

    property point scaledStartPoint: Qt.point((path.startX - obj.x) * scaleFactor, (path.startY - obj.y) * scaleFactor)
    property point scaledEndPoint: Qt.point((pathCubic.x - obj.x) * scaleFactor, (pathCubic.y - obj.y) * scaleFactor)

    function innerStrokeStyle(ctx): color {
        // if (activeFocus)
        //     return obj.context.styles.connection.selectedColor;
        if (!obj.context.styles.connection.useDataDefinedColors)
            return obj.context.styles.connection.normalColor;
        if (inTypeColor == outTypeColor)
            return inTypeColor;

        const gradient = ctx.createLinearGradient(scaledStartPoint.x, scaledStartPoint.y, scaledEndPoint.x, scaledEndPoint.y);

        for (let i = 0; i != 10; i++) {
            const color = obj.context.styles.connection.lerpOklabColors(inTypeColor, outTypeColor, i / 10);
            gradient.addColorStop(i / 10, color);
        }

        return gradient;
    }

    onPaint: {
        let ctx = getContext('2d');
        ctx.clearRect(0, 0, width, height);

        const c1 = Qt.point((pathCubic.control1X - obj.x) * scaleFactor, (pathCubic.control1Y - obj.y) * scaleFactor);
        const c2 = Qt.point((pathCubic.control2X - obj.x) * scaleFactor, (pathCubic.control2Y - obj.y) * scaleFactor);

        ctx.beginPath();
        ctx.lineCap = "round";
        ctx.moveTo(scaledStartPoint.x, scaledStartPoint.y);
        ctx.bezierCurveTo(c1.x, c1.y, c2.x, c2.y, scaledEndPoint.x, scaledEndPoint.y);

        if (obj.fullyConnected) {
            ctx.setLineDash([]);
            ctx.lineWidth = obj.context.styles.connection.lineWidth * scaleFactor;
            if (obj.activeFocus) {
                ctx.strokeStyle = obj.context.styles.connection.selectedHaloColor;
                ctx.stroke();
            } else if (obj.hovered) {
                ctx.strokeStyle = obj.context.styles.connection.hoveredColor;
                ctx.stroke();
            }
            ctx.strokeStyle = innerStrokeStyle(ctx);
            ctx.lineWidth = obj.context.styles.connection.lineWidth * 0.45 * scaleFactor;
            ctx.stroke();
        } else {
            ctx.setLineDash([6, 2]);
            ctx.lineWidth = obj.context.styles.connection.constructionLineWidth * scaleFactor;
            ctx.strokeStyle = obj.context.styles.connection.constructionColor;
            ctx.stroke();
        }
    }

    Path {
        id: path

        property alias dc: defaultConnectionPainter

        property bool swap: dc.obj.connection.inNodeId === NodeEditorUtils.InvalidNodeId

        startX: swap ? dc.obj.outPoint.x : dc.obj.inPoint.x
        startY: swap ? dc.obj.outPoint.y : dc.obj.inPoint.y
        PathCubic {
            id: pathCubic

            property alias dc: defaultConnectionPainter
            x: path.swap ? dc.obj.inPoint.x : dc.obj.outPoint.x
            y: path.swap ? dc.obj.inPoint.y : dc.obj.outPoint.y
            control1X: path.swap ? dc.c2.x : dc.c1.x
            control1Y: path.swap ? dc.c2.y : dc.c1.y
            control2X: path.swap ? dc.c1.x : dc.c2.x
            control2Y: path.swap ? dc.c1.y : dc.c2.y
        }
    }
}
