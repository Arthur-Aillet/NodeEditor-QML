import QtQuick
import QtQuick.Shapes 1.11
import NodeModule

Shape {
    id: root
    width: 100
    height: 50

    required property real nodeId
    required property nodeStyle style
    required property bool selected
    required property bool hovered

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
