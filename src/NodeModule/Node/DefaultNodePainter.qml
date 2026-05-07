pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes
import NodeModule

Shape {
    id: root

    required property NodeGraphicalObject nodeObject
    required property NavigableArea area
    required property DraftConnection draftConnection

    property size size: ModelInterface.nodeGeometry.size(nodeObject.nodeId)

    width: size.width
    height: size.height

    property real strokeWidth: nodeObject.hovered ? nodeObject.style.hoveredPenWidth : nodeObject.style.penWidth

    function boundaryColor(): color {
        // if (invalid) {
        //     return errorColor;
        // } else if (warning) {
        //     return warningColor;
        // } else
        if (nodeObject.selected) {
            return nodeObject.style.selectedBoundaryColor;
        } else {
            return nodeObject.style.normalBoundaryColor;
        }
    }

    preferredRendererType: Shape.CurveRenderer

    ShapePath {
        strokeWidth: root.strokeWidth
        strokeColor: root.boundaryColor()
        fillGradient: LinearGradient {
            x1: 0.0
            y1: 0.0
            x2: 2.0
            y2: root.height
            stops: [
                GradientStop {
                    position: 0.0
                    color: root.nodeObject.style.gradientColor0
                },
                GradientStop {
                    position: 0.10
                    color: root.nodeObject.style.gradientColor1
                },
                GradientStop {
                    position: 0.9
                    color: root.nodeObject.style.gradientColor2
                },
                GradientStop {
                    position: 1.0
                    color: root.nodeObject.style.gradientColor3
                }
            ]
        }
        PathRectangle {
            width: root.width
            height: root.height
            radius: 3.0
        }
    }

    Repeater {
        id: inOutRepeater
        property int inPortCount: ModelInterface.nodeData(root.nodeObject.nodeId, NodeRole.InPortCount)
        model: inPortCount + ModelInterface.nodeData(root.nodeObject.nodeId, NodeRole.OutPortCount)
        delegate: Item {
            id: port
            required property int index
            property int portId: index % inOutRepeater.inPortCount
            property var side: (index < inOutRepeater.inPortCount) ? PortType.In : PortType.Out

            property bool connected: false
            property var dataType: ModelInterface.portData(root.nodeObject.nodeId, side, port.portId, PortRole.DataType)

            Text {
                id: portLabel
                property bool captionVisible: ModelInterface.portData(root.nodeObject.nodeId, port.side, port.portId, PortRole.CaptionVisible)
                property string caption: ModelInterface.portData(root.nodeObject.nodeId, port.side, port.portId, PortRole.Caption)

                property point pos: ModelInterface.nodeGeometry.portTextPosition(root.nodeObject.nodeId, port.side, port.portId)

                FontMetrics {
                    id: portLabelMetrics
                    font.family: portLabel.font.family
                }

                x: pos.x
                y: pos.y - portLabelMetrics.ascent
                text: captionVisible ? caption : port.dataType.name
                color: port.connected ? root.nodeObject.style.fontColor : root.nodeObject.style.fontColorFaded
            }

            Shape {
                id: connectionPoint
                property point pos: ModelInterface.nodeGeometry.portPosition(root.nodeObject.nodeId, port.side, port.portId)

                x: pos.x
                y: pos.y

                preferredRendererType: Shape.CurveRenderer

                ShapePath {
                    fillColor: root.nodeObject.style.connectionPointColor
                    strokeWidth: root.strokeWidth
                    strokeColor: root.boundaryColor()

                    PathAngleArc {
                        property real radius: root.nodeObject.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

                        Binding on radius {
                            when: root.draftConnection.selectedPort !== null
                            value: {
                                if (root.draftConnection.selectedPort === null || root.draftConnection.selectedPort.portType == port.side)
                                    return root.nodeObject.style.connectionPointDiameter * 0.6;
                                const pos = Qt.point(root.nodeObject.x + connectionPoint.pos.x, root.nodeObject.y + connectionPoint.pos.y);
                                const diff = Qt.vector2d(root.area.mousePosition.x - pos.x, root.area.mousePosition.y - pos.y);
                                const dist = Math.sqrt(diff.dotProduct(diff));

                                let r;
                                if (root.draftConnection.selectedPort.nodeId != root.nodeObject.nodeId) {
                                    const thres = 40.0;
                                    r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
                                } else {
                                    const thres = 80.0;
                                    r = (dist < thres) ? (dist / thres) : 1.0;
                                }
                                return root.nodeObject.style.connectionPointDiameter * 0.6 * r;
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
    }

    property string label: ModelInterface.nodeData(nodeObject.nodeId, NodeRole.Label)
    property bool labelEditable: ModelInterface.nodeData(nodeObject.nodeId, NodeRole.LabelEditable)
    property string caption: ModelInterface.nodeData(nodeObject.nodeId, NodeRole.Caption)

    property var capPos: ModelInterface.nodeGeometry.captionPosition(nodeObject.nodeId)
    property var capRect: ModelInterface.nodeGeometry.captionRect(nodeObject.nodeId)

    FontMetrics {
        id: fontMetrics
        font.family: captionText.font.family
        font.bold: captionText.font.bold
        font.italic: captionText.font.italic
    }

    Text {
        id: captionText
        text: root.caption
        color: root.nodeObject.style.fontColor
        font.bold: root.label == ""
        font.italic: root.label != ""
        visible: ModelInterface.nodeData(root.nodeObject.nodeId, NodeRole.CaptionVisible)

        x: parent.capPos.x + parent.capRect.width / 2.0 - fontMetrics.boundingRect(root.caption).width / 2.0
        y: parent.capPos.y - fontMetrics.ascent
    }

    Text {
        text: root.label
        color: root.nodeObject.style.fontColor
        visible: ModelInterface.nodeData(root.nodeObject.nodeId, NodeRole.LabelVisible)
        anchors.horizontalCenter: parent.horizontalCenter

        x: parent.capPos.x + parent.capRect.width / 2.0
        y: parent.capPos.y - fontMetrics.height - 2.0 - fontMetrics.ascent
    }
}
