pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes 1.11
import NodeModule

Item {
    id: root
    width: 140
    height: 80

    required property real nodeId
    required property nodeStyle style
    
    required property bool selected
    required property bool hovered

    property string label: ModelInterface.nodeData(nodeId, NodeRole.Label)
    property bool labelVisible: ModelInterface.nodeData(nodeId, NodeRole.LabelVisible)
    property bool labelEditable: ModelInterface.nodeData(nodeId, NodeRole.LabelEditable)
    property string caption: ModelInterface.nodeData(nodeId, NodeRole.Caption)
    property bool captionVisible:ModelInterface.nodeData(nodeId, NodeRole.CaptionVisible)
    
    property real ports: 3

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

    Shape {
        ShapePath {
            strokeWidth: root.hovered ? root.style.hoveredPenWidth : root.style.penWidth
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
            PathLine {
                x: 0
                y: 0
            }
            PathLine {
                x: root.width
                y: 0
            }
            PathLine {
                x: root.width
                y: root.height
            }
            PathLine {
                x: 0
                y: root.height
            }
            PathLine {
                x: 0
                y: 0
            }
        }
    }

    Repeater {
        id: inOutRepeater
        model: 2
        delegate: Repeater {
            id: port
            property bool inPort: (port.index == 0)
            property var type: port.inPort ? PortType.In : PortType.Out
            model: ModelInterface.nodeData(root.nodeId, inPort ? NodeRole.InPortCount : NodeRole.OutPortCount)
            required property int index
            delegate: Item {
                id: connection
                required property int index
                property bool connected: false
                property string caption: ModelInterface.portData(root.nodeId, port.type, connection.index, PortRole.Caption)
                property bool captionVisible: ModelInterface.portData(root.nodeId, port.type, connection.index, PortRole.CaptionVisible)
                property var dataType: ModelInterface.portData(root.nodeId, port.type, connection.index, PortRole.DataType)

                x: port.inPort ? 0 : root.width
                y: connection.index * 20 + 30
                height: root.style.connectionPointDiameter

                Text {
                    anchors.verticalCenter: parent.verticalCenter 
                    anchors.left: port.inPort ? parent.right : undefined
                    anchors.right: port.inPort ? undefined : parent.left
                    topPadding: -5
                    leftPadding: 8
                    rightPadding: 8
                    text: connection.dataType.name
                    horizontalAlignment: port.inPort ? Text.AlignLeft : Text.AlignRight
                    color: connection.connected ? root.style.fontColor : root.style.fontColorFaded
                }

                Shape {
                    containsMode: Shape.BoundingRectContains
                    ShapePath {
                        fillColor: root.style.connectionPointColor
                        strokeWidth: hoverHandler.hovered ? root.style.hoveredPenWidth : root.style.penWidth
                        strokeColor: root.boundaryColor()
                        PathAngleArc {
                            radiusX: root.style.connectionPointDiameter / 2
                            radiusY: root.style.connectionPointDiameter / 2
                            startAngle: 0
                            sweepAngle: 360
                        }
                    }
                    HoverHandler {
                        id: hoverHandler
                    }
                    TapHandler {
                        onTapped: print("Connection clicked")
                    }
                }
            }
        }
    }

    Text {
        text: root.caption
        color: root.style.fontColor
        font.bold: root.label == ""
        font.italic: root.label != ""
        visible: root.captionVisible
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 3
    }
    Text {
        text: root.label
        color: root.style.fontColor
        visible: root.labelVisible
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 3
    }
}
