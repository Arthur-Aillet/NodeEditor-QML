import QtQuick
import QtQuick.Shapes
import CutieDesigner.Nodes.Display

Shape {
    id: fillSurface
    anchors.fill: parent
    required property FillNode node

    Component {
        id: stopComponent
        GradientStop {}
    }

    Connections {
        target: fillSurface.node
        function onGradientChanged() {
            path.computeGradient();
        }
    }

    ShapePath {
        id: path
        startX: fillSurface.x
        startY: fillSurface.y

        strokeWidth: 0

        PathLine {
            x: fillSurface.x
            y: fillSurface.y + fillSurface.height
        }
        PathLine {
            x: fillSurface.x + fillSurface.width
            y: fillSurface.y + fillSurface.height
        }
        PathLine {
            x: fillSurface.x + fillSurface.width
            y: fillSurface.y
        }
        PathLine {
            x: fillSurface.x
            y: fillSurface.y
        }

        Component.onCompleted: {
            computeGradient();
        }

        function computeGradient() {
            let stopsList = [];

            for (let i = 0; i < fillSurface.node.gradient.length; i++) {
                let s1 = stopComponent.createObject(fillSurface, {
                    "position": fillSurface.node.gradient[i][0],
                    "color": fillSurface.node.gradient[i][1]
                });
                stopsList.push(s1);
            }
            gradient.stops = stopsList;
            gradient.y2 += 10;
            gradient.y2 -= 10;
        }

        fillGradient: LinearGradient {
            id: gradient
            x1: fillSurface.x + fillSurface.node.start.x
            y1: fillSurface.y + fillSurface.node.start.y
            x2: fillSurface.x + (fillSurface.node.endSet ? fillSurface.node.end.x : fillSurface.width)
            y2: fillSurface.y + fillSurface.node.end.y
        }
    }
}
