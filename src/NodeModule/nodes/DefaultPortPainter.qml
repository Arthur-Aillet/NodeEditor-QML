import QtQuick
import QtQuick.Shapes
import NodeModule

Item {
    id: port
    required property int index
    required property DefaultNodePainter nodePainter

    width: portLabel.width + portLabel.offset

    property var style: nodePainter.nodeObject.style
    property int side: (index < nodePainter.nodeObject.inPortCount) ? NodeEditor.PortType.In : NodeEditor.PortType.Out
    property int portId: side == NodeEditor.PortType.In ? index : index - nodePainter.nodeObject.inPortCount

    signal portConnectionChanged

    onPortConnectionChanged: {
        connected = ModelInterface.graph.connections(nodePainter.nodeObject.nodeId, side, portId).length != 0;
    }

    property bool connected: ModelInterface.graph.connections(nodePainter.nodeObject.nodeId, side, portId).length != 0
    property var dataType: ModelInterface.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.DataType)

    Text {
        id: portLabel
        readonly property real offset: 10

        property bool captionVisible: ModelInterface.portData(port.nodePainter.nodeObject.nodeId, port.side, port.portId, NodeEditor.PortRole.CaptionVisible)
        property string caption: ModelInterface.portData(port.nodePainter.nodeObject.nodeId, port.side, port.portId, NodeEditor.PortRole.Caption)

        FontMetrics {
            id: portLabelMetrics
            font.family: portLabel.font.family
        }

        x: connectionPoint.x + (port.side == NodeEditor.PortType.In ? offset : -width - offset)
        y: connectionPoint.y - portLabelMetrics.ascent / 2.0 - port.style.connectionPointDiameter / 4.0
        text: captionVisible ? caption : port.dataType.name
        color: port.connected ? port.style.fontColor : port.style.fontColorFaded
    }

    Shape {
        id: connectionPoint
        property point pos: (port.nodePainter as AbstractNodePainter).getPortPosition(port.portId, port.side)

        x: pos.x
        y: pos.y

        preferredRendererType: Shape.CurveRenderer

        ShapePath {
            fillColor: port.connected ? port.style.filledConnectionPointColor : port.style.connectionPointColor
            strokeWidth: port.nodePainter.strokeWidth
            strokeColor: port.nodePainter.strokeColor

            PathAngleArc {
                property real radius: port.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

                Binding on radius {
                    property alias painter: port.nodePainter

                    when: painter.draftConnection.selectedPort !== null && !port.connected
                    value: {
                        if (painter.draftConnection.selectedPort === null || painter.draftConnection.selectedPort.portType === port.side)
                            return port.style.connectionPointDiameter * 0.6;
                        const pos = Qt.point(painter.nodeObject.x + connectionPoint.x, painter.nodeObject.y + connectionPoint.y);
                        const diff = Qt.vector2d(painter.area.mousePosition.x - pos.x, painter.area.mousePosition.y - pos.y);
                        const dist = Math.sqrt(diff.dotProduct(diff));

                        let r;
                        if (painter.draftConnection.selectedPort.nodeId != painter.nodeObject.nodeId) {
                            const thres = 40.0;
                            r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
                        } else {
                            const thres = 80.0;
                            r = (dist < thres) ? (dist / thres) : 1.0;
                        }
                        return port.style.connectionPointDiameter * 0.6 * r;
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
