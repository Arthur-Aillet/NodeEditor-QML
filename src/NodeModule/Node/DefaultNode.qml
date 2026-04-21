import QtQuick
import QtQuick.Shapes 1.11
import NodeModule

Shape {
    id: root
    width: 100
    height: 50

    required property nodeStyle style
    required property bool selected

    ShapePath {
        strokeWidth: root.penWidth
        strokeColor: root.boundaryColor
        fillGradient: LinearGradient {
            x1: 0.0
            y1: 0.0
            x2: 2.0
            y2: root.height
            stops: [
                GradientStop {
                    position: 0.0
                    color: root.gradientColor0
                },
                GradientStop {
                    position: 0.10
                    color: root.style.normalBoundaryColor
                },
                GradientStop {
                    position: 0.9
                    color: root.gradientColor2
                },
                GradientStop {
                    position: 1.0
                    color: root.gradientColor3
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

    property bool invalid: false
    property bool warning: false //TODO CONVERT TO ENUM
    property color normalBoundaryColor: "blue"
    property color selectedBoundaryColor: "red"
    property color errorColor: "green"
    property color warningColor: "yellow"

    property real hoveredPenWidth: 4
    property real penWidth: 3

    property color gradientColor0: "yellow"
    property color gradientColor1: "green"
    property color gradientColor2: "red"
    property color gradientColor3: "blue"

    // property color gradientColor0: "gray"
    // property color gradientColor1: [80, 80, 80]
    // property color gradientColor2: [64, 64, 64]
    // property color gradientColor3: [58, 58, 58]

    property color boundaryColor: getBoundaryColor()

    function getBoundaryColor(): color {
        if (invalid) {
            return errorColor;
        } else if (warning) {
            return warningColor;
        } else if (selected) {
            return selectedBoundaryColor;
        } else {
            return normalBoundaryColor;
        }
    }
}
