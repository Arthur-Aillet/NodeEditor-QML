import QtQuick
import QtQuick.Shapes
import NodeModule

Item {
    id: port
    required property int index
    required property DefaultNodePainter nodePainter

    property int portId: index % nodePainter.nodeObject.inPortCount
    property var side: (index < nodePainter.nodeObject.inPortCount) ? PortType.In : PortType.Out

    property bool connected: false
    property var dataType: ModelInterface.portData(nodePainter.nodeObject.nodeId, side, port.portId, PortRole.DataType)

    Text {
        id: portLabel
        property bool captionVisible: ModelInterface.portData(port.nodePainter.nodeObject.nodeId, port.side, port.portId, PortRole.CaptionVisible)
        property string caption: ModelInterface.portData(port.nodePainter.nodeObject.nodeId, port.side, port.portId, PortRole.Caption)

        property point pos: ModelInterface.nodeGeometry.portTextPosition(port.nodePainter.nodeObject.nodeId, port.side, port.portId)

        FontMetrics {
            id: portLabelMetrics
            font.family: portLabel.font.family
        }

        x: pos.x
        y: pos.y - portLabelMetrics.ascent
        text: captionVisible ? caption : port.dataType.name
        color: port.connected ? port.nodePainter.nodeObject.style.fontColor : port.nodePainter.nodeObject.style.fontColorFaded
    }

    Shape {
        id: connectionPoint
        property point pos: ModelInterface.nodeGeometry.portPosition(port.nodePainter.nodeObject.nodeId, port.side, port.portId)

        x: pos.x
        y: pos.y

        preferredRendererType: Shape.CurveRenderer

        ShapePath {
            fillColor: port.nodePainter.nodeObject.style.connectionPointColor
            strokeWidth: port.nodePainter.strokeWidth
            strokeColor: port.nodePainter.strokeColor

            PathAngleArc {
                property real radius: port.nodePainter.nodeObject.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

                Binding on radius {
                    when: port.nodePainter.draftConnection.selectedPort !== null
                    value: {
                        if (port.nodePainter.draftConnection.selectedPort === null || port.nodePainter.draftConnection.selectedPort.portType == port.side)
                            return port.nodePainter.nodeObject.style.connectionPointDiameter * 0.6;
                        const pos = Qt.point(port.nodePainter.nodeObject.x + connectionPoint.pos.x, port.nodePainter.nodeObject.y + connectionPoint.pos.y);
                        const diff = Qt.vector2d(port.nodePainter.area.mousePosition.x - pos.x, port.nodePainter.area.mousePosition.y - pos.y);
                        const dist = Math.sqrt(diff.dotProduct(diff));

                        let r;
                        if (port.nodePainter.draftConnection.selectedPort.nodeId != port.nodePainter.nodeObject.nodeId) {
                            const thres = 40.0;
                            r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
                        } else {
                            const thres = 80.0;
                            r = (dist < thres) ? (dist / thres) : 1.0;
                        }
                        return port.nodePainter.nodeObject.style.connectionPointDiameter * 0.6 * r;
                    }
                }
                radiusX: radius
                radiusY: radius
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
}
