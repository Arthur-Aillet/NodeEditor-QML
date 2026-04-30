pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes 1.11
import NodeModule

Shape {
    id: root

    required property real nodeId
    required property nodeStyle style

    property size size: ModelInterface.nodeGeometry.size(nodeId)

    width: size.width
    height: size.height

    required property bool selected
    required property bool hovered

    property real strokeWidth: root.hovered ? root.style.hoveredPenWidth : root.style.penWidth

    function boundaryColor(): color {
        // if (invalid) {
        //     return errorColor;
        // } else if (warning) {
        //     return warningColor;
        // } else
        if (selected) {
            return style.selectedBoundaryColor;
        } else {
            return style.normalBoundaryColor;
        }
    }

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
                    color: root.style.gradientColor0
                },
                GradientStop {
                    position: 0.10
                    color: root.style.gradientColor1
                },
                GradientStop {
                    position: 0.9
                    color: root.style.gradientColor2
                },
                GradientStop {
                    position: 1.0
                    color: root.style.gradientColor3
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
        property int inPortCount: ModelInterface.nodeData(root.nodeId, NodeRole.InPortCount)
        model: inPortCount + ModelInterface.nodeData(root.nodeId, NodeRole.OutPortCount)
        delegate: Item {
            id: port
            required property int index
            property int portId: index % inOutRepeater.inPortCount
            property var side: (index < inOutRepeater.inPortCount) ? PortType.In : PortType.Out

            property bool connected: false
            property var dataType: ModelInterface.portData(root.nodeId, side, port.portId, PortRole.DataType)

            Text {
                id: portLabel
                property bool captionVisible: ModelInterface.portData(root.nodeId, port.side, port.portId, PortRole.CaptionVisible)
                property string caption: ModelInterface.portData(root.nodeId, port.side, port.portId, PortRole.Caption)

                property point pos: ModelInterface.nodeGeometry.portTextPosition(root.nodeId, port.side, port.portId)

                FontMetrics {
                    id: portLabelMetrics
                    font.family: portLabel.font.family
                }

                x: pos.x
                y: pos.y - portLabelMetrics.ascent
                text: captionVisible ? caption : port.dataType.name
                color: port.connected ? root.style.fontColor : root.style.fontColorFaded
            }

            Shape {
                property point pos: ModelInterface.nodeGeometry.portPosition(root.nodeId, port.side, port.portId)

                x: pos.x
                y: pos.y

                ShapePath {
                    fillColor: root.style.connectionPointColor
                    strokeWidth: root.strokeWidth
                    strokeColor: root.boundaryColor()
                    PathAngleArc {
                        property real radius: root.style.connectionPointDiameter * 0.6 // Diameter is used a the radius in the original

                        radiusX: radius
                        radiusY: radius
                        startAngle: 0
                        sweepAngle: 360
                    }
                }
            }
        }
    }

    property string label: ModelInterface.nodeData(nodeId, NodeRole.Label)
    property bool labelEditable: ModelInterface.nodeData(nodeId, NodeRole.LabelEditable)
    property string caption: ModelInterface.nodeData(nodeId, NodeRole.Caption)

    property var capPos: ModelInterface.nodeGeometry.captionPosition(root.nodeId)
    property var capRect: ModelInterface.nodeGeometry.captionRect(root.nodeId)

    FontMetrics {
        id: fontMetrics
        font.family: captionText.font.family
        font.bold: captionText.font.bold
        font.italic: captionText.font.italic
    }

    Text {
        id: captionText
        text: root.caption
        color: root.style.fontColor
        font.bold: root.label == ""
        font.italic: root.label != ""
        visible: ModelInterface.nodeData(root.nodeId, NodeRole.CaptionVisible)

        x: parent.capPos.x + parent.capRect.width / 2.0 - fontMetrics.boundingRect(root.caption).width / 2.0
        y: parent.capPos.y - fontMetrics.ascent
    }

    Text {
        text: root.label
        color: root.style.fontColor
        visible: ModelInterface.nodeData(root.nodeId, NodeRole.LabelVisible)
        anchors.horizontalCenter: parent.horizontalCenter

        x: parent.capPos.x + parent.capRect.width / 2.0
        y: parent.capPos.y - fontMetrics.height - 2.0 - fontMetrics.ascent
    }
}
