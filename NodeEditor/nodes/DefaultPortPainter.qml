import QtQuick
import QtQuick.Shapes
import NodeEditor

Item {
    id: port
    required property int index
    required property AbstractNodePainter nodePainter

    width: portLabel.width + portLabel.offset

    property var style: nodePainter.nodeObject.style
    property int side: (index < nodePainter.nodeObject.inPortCount) ? NodeEditor.PortSide.In : NodeEditor.PortSide.Out
    property int portId: side == NodeEditor.PortSide.In ? index : index - nodePainter.nodeObject.inPortCount

    signal portConnectionChanged

    onPortConnectionChanged: {
        connected = nodePainter.context.graphModel.connections(nodePainter.nodeObject.nodeId, side, portId).length != 0;
    }

    property bool connected
    property nodeDataType dataType
    property bool captionVisible
    property string caption

    function queryPortInfo() {
        connected = nodePainter.context.graphModel.connections(nodePainter.nodeObject.nodeId, side, portId).length != 0;
        dataType = nodePainter.context.graphModel.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.DataType);
        captionVisible = nodePainter.context.graphModel.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.PortCaptionVisible);
        caption = nodePainter.context.graphModel.portData(nodePainter.nodeObject.nodeId, side, portId, NodeEditor.PortRole.PortCaption);
    }

    Component.onCompleted: port.queryPortInfo()

    Connections {
        target: port.nodePainter.nodeObject
        function onPortsChanged(side: int) {
            if (side == port.side) {
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

        x: connectionPoint.x + (port.side == NodeEditor.PortSide.In ? offset : -width - offset)
        y: connectionPoint.y - portLabelMetrics.ascent / 2.0 - port.style.connectionPointDiameter / 4.0
        text: port.captionVisible ? port.caption : port.dataType.name
        color: port.connected ? port.style.fontColor : port.style.fontColorFaded
    }

    Shape {
        id: connectionPoint
        property point pos: port.nodePainter.getPortPosition(port.portId, port.side)

        x: pos.x
        y: pos.y

        preferredRendererType: Shape.CurveRenderer

        ShapePath {
            fillColor: {
                if (port.nodePainter.nodeObject.context.styles.connection.useDataDefinedColors) {
                    const color = port.nodePainter.nodeObject.context.styles.connection.typeColor(port.dataType.id);
                    if (!port.connected) {
                        color.a = 0.7;
                    }
                    return color;
                } else {
                    return port.connected ? port.style.filledConnectionPointColor : port.style.connectionPointColor;
                }
            }
            strokeWidth: port.nodePainter.nodeObject.containsMouse ? port.nodePainter.nodeObject.style.hoveredPenWidth : port.nodePainter.nodeObject.style.penWidth
            strokeColor: {
                if (port.nodePainter.nodeObject.locked)
                    return Qt.darker(port.nodePainter.nodeObject.style.normalBoundaryColor, 1.4);
                if (port.nodePainter.nodeObject.selected) {
                    return port.nodePainter.nodeObject.style.selectedBoundaryColor;
                } else {
                    return port.nodePainter.nodeObject.style.normalBoundaryColor;
                }
            }

            PathAngleArc {
                property real radius: port.style.connectionPointDiameter * 0.6 // Diameter is used as the radius in the original

                Binding on radius {
                    property alias painter: port.nodePainter
                    property var connectionPossible: {
                        if (painter.draftConnection.selectedPort === null)
                            return false;
                        if (painter.draftConnection.selectedPort.portSide === NodeEditor.PortSide.In) {
                            return port.nodePainter.context.graphModel.connectionPossible({
                                inNodeId: painter.draftConnection.selectedPort.nodeId,
                                inPortIndex: painter.draftConnection.selectedPort.portId,
                                outNodeId: painter.nodeObject.nodeId,
                                outPortIndex: port.portId
                            });
                        } else {
                            return port.nodePainter.context.graphModel.connectionPossible({
                                inNodeId: painter.nodeObject.nodeId,
                                inPortIndex: port.portId,
                                outNodeId: painter.draftConnection.selectedPort.nodeId,
                                outPortIndex: painter.draftConnection.selectedPort.portId
                            });
                        }
                    }

                    when: painter.draftConnection.selectedPort !== null && painter.draftConnection.selectedPort.portSide !== port.side
                    value: {
                        const pos = Qt.point(painter.nodeObject.x + connectionPoint.x, painter.nodeObject.y + connectionPoint.y);
                        const diff = Qt.vector2d(painter.area.mousePosition.x - pos.x, painter.area.mousePosition.y - pos.y);
                        const dist = Math.sqrt(diff.dotProduct(diff));

                        let r;
                        if (connectionPossible) {
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
