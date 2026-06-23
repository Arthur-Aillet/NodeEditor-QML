import QtQuick
import QtQuick.Shapes
import NodeEditor

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

    property bool connected
    property string dataType
    property string dataName
    property bool captionVisible
    property string caption

    function queryPortInfo() {
        connected = ModelInterface.graph.connections(nodePainter.nodeObject.nodeId, side, portId).length != 0;
        dataType = ModelInterface.graph.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.DataType);
        dataName = ModelInterface.graph.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.DataName);
        captionVisible = ModelInterface.graph.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.PortCaptionVisible);
        caption = ModelInterface.graph.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.PortCaption);
    }

    Component.onCompleted: port.queryPortInfo()

    Connections {
        target: port.nodePainter.nodeObject
        function onPortsChanged(type: int) {
            if (type == port.side) {
                port.queryPortInfo();
            }
        }
    }

    Text {
        id: portLabel
        readonly property real offset: 10

        FontMetrics {
            id: portLabelMetrics
            font.family: portLabel.font.family
        }

        x: connectionPoint.x + (port.side == NodeEditor.PortType.In ? offset : -width - offset)
        y: connectionPoint.y - portLabelMetrics.ascent / 2.0 - port.style.connectionPointDiameter / 4.0
        text: port.captionVisible ? port.caption : port.dataName
        color: port.connected ? port.style.fontColor : port.style.fontColorFaded
    }

    Shape {
        id: connectionPoint
        property point pos: (port.nodePainter as AbstractNodePainter).getPortPosition(port.portId, port.side)

        x: pos.x
        y: pos.y

        preferredRendererType: Shape.CurveRenderer

        ShapePath {
            fillColor: {
                if (StyleCollection.connection.useDataDefinedColors) {
                    const color = StyleCollection.connection.typeColor(port.dataType);
                    if (!port.connected) {
                        color.a = 0.7;
                    }
                    return color;
                } else {
                    return port.connected ? port.style.filledConnectionPointColor : port.style.connectionPointColor;
                }
            }
            strokeWidth: port.nodePainter.strokeWidth
            strokeColor: port.nodePainter.strokeColor

            PathAngleArc {
                property real radius: port.style.connectionPointDiameter * 0.6 // Diameter is used as the radius in the original

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
