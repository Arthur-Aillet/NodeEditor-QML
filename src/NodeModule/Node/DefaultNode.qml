pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes 1.11
import NodeModule
import QtQuick.Shapes.DesignHelpers

Item {
    id: root
    width: 100
    height: 50

    required property real nodeId
    required property nodeStyle style
    required property bool selected
    required property bool hovered

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
            id: portsRepeater
            property bool inPorts: (portsRepeater.index == 0)
            model: ModelInterface.nodeData(root.nodeId, inPorts ? NodeRole.InPortCount : NodeRole.OutPortCount)
            required property int index
            delegate: EllipseShape {
                id: connection
                required property int index
                fillColor: root.style.connectionPointColor
                x: portsRepeater.inPorts ? 0 : root.width
                y: index * 12 + 10
                width: root.style.connectionPointDiameter
                height: root.style.connectionPointDiameter
            }
        }
    }
}
