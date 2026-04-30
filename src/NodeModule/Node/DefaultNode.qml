pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes 1.11
import NodeModule

Item {
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

    Shape {
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

                Text {
                    id: portLabel
                    property point pos: ModelInterface.nodeGeometry.portTextPosition(root.nodeId, port.type, connection.index)

                    FontMetrics {
                        id: portLabelMetrics
                        font.family: portLabel.font.family
                        font.bold: portLabel.font.bold
                        font.italic: portLabel.font.italic
                    }

                    x: pos.x
                    y: pos.y - portLabelMetrics.ascent
                    text: connection.dataType.name
                    color: connection.connected ? root.style.fontColor : root.style.fontColorFaded
                }

                Shape {
                    property point pos: ModelInterface.nodeGeometry.portPosition(root.nodeId, port.type, connection.index)

                    x: pos.x
                    y: pos.y

                    containsMode: Shape.BoundingRectContains
                    ShapePath {
                        fillColor: root.style.connectionPointColor
                        strokeWidth: root.strokeWidth
                        strokeColor: root.boundaryColor()
                        PathAngleArc {
                            radiusX: root.style.connectionPointDiameter / 2
                            radiusY: root.style.connectionPointDiameter / 2
                            startAngle: 0
                            sweepAngle: 360
                        }
                    }

                    DragHandler {
                        target: null
                        grabPermissions: PointerHandler.CanTakeOverFromAnything | PointerHandler.ApprovesTakeOverByHandlersOfSameType
                        cursorShape: Qt.CrossCursor
                        onCentroidChanged: () => {
                            console.log("moved: ");
                        }
                        onCanceled: {
                            console.log("ended");
                        }
                        onGrabChanged: e => {
                            console.log("grabchagned " + e);
                        }
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
        font.family: caption.font.family
        font.bold: caption.font.bold
        font.italic: caption.font.italic
    }

    Text {
        id: caption
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
