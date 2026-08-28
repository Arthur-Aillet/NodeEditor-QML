import QtQuick
import NodeEditor

AbstractConnectionPainter {
    id: painter

    Connections {
        target: painter.obj
        function onHoveredChanged() {
            painter.markDirty(Qt.rect(painter.x, painter.y, painter.width, painter.height));
        }

        function onActiveFocusChanged() {
            painter.markDirty(Qt.rect(painter.x, painter.y, painter.width, painter.height));
        }
    }

    override property var distanceToCurve: point => {
        const a = Math.abs((endY - startY) * point.x - (endX - startX) * point.y + endX * startY - endY * startX);
        const b = Math.sqrt((endY - startY) * (endY - startY) + (endX - startX) * (endX - startX));
        return a / b;
    }

    width: obj.width
    height: obj.height

    override property rect boundingBox: {
        const bbx = Math.min(obj.inPoint.x, obj.outPoint.x) - style.lineWidth;
        const bby = Math.min(obj.inPoint.y, obj.outPoint.y) - style.lineWidth;

        return {
            x: bbx,
            y: bby,
            width: (Math.max(obj.inPoint.x, obj.outPoint.x) - bbx) + style.lineWidth * 3,
            height: (Math.max(obj.inPoint.y, obj.outPoint.y) - bby) + style.lineWidth * 3
        };
    }

    property bool swap: obj.connection.inNodeId === NodeEditorUtils.InvalidNodeId
    property real startX: swap ? obj.outPoint.x : obj.inPoint.x
    property real startY: swap ? obj.outPoint.y : obj.inPoint.y
    property real endX: swap ? obj.inPoint.x : obj.outPoint.x
    property real endY: swap ? obj.inPoint.y : obj.outPoint.y
    property point scaledStartPoint: Qt.point((startX - obj.x), (startY - obj.y))
    property point scaledEndPoint: Qt.point((endX - obj.x), (endY - obj.y))

    onPaint: {
        let ctx = getContext('2d');
        ctx.clearRect(0, 0, width, height);

        ctx.beginPath();
        ctx.lineCap = "round";
        ctx.moveTo(scaledStartPoint.x, scaledStartPoint.y);
        ctx.lineTo(scaledEndPoint.x, scaledEndPoint.y);

        if (obj.fullyConnected) {
            ctx.setLineDash([]);
            ctx.lineWidth = style.lineWidth;
            if (obj.activeFocus) {
                ctx.strokeStyle = style.selectedHaloColor;
                ctx.stroke();
            } else if (obj.hovered) {
                ctx.strokeStyle = style.hoveredColor;
                ctx.stroke();
            }
            const dataType = obj.context.graphModel.portData(obj.connection.inNodeId, NodeEditor.PortSide.In, obj.connection.inPortIndex, NodeEditor.PortRole.DataType);
            ctx.strokeStyle = style.typeColor(dataType.id);
            ctx.lineWidth = style.lineWidth * 0.45;
            ctx.stroke();
        } else {
            ctx.setLineDash([6, 2]);
            ctx.lineWidth = style.constructionLineWidth;
            ctx.strokeStyle = style.constructionColor;
            ctx.stroke();
        }
        ctx.closePath();
    }
}
