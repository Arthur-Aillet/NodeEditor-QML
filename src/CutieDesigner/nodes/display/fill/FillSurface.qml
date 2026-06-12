import QtQuick
import QtQuick.Shapes
import CutieDesigner.Nodes

Shape {
    id: root
    anchors.fill: parent
    required property FillNode node

    Component {
        id: stopComponent
        GradientStop {}
    }

    Connections {
        target: root.node
        function onGradientChanged() {
            path.computeGradient();
        }
    }

    ShapePath {
        id: path
        startX: root.x
        startY: root.y

        strokeWidth: 0

        PathLine {
            x: root.x
            y: root.y + root.height
        }
        PathLine {
            x: root.x + root.width
            y: root.y + root.height
        }
        PathLine {
            x: root.x + root.width
            y: root.y
        }
        PathLine {
            x: root.x
            y: root.y
        }

        Component.onCompleted: {
            computeGradient();
        }

        function computeGradient() {
            let stopsList = [];

            for (let i = 0; i < root.node.gradient.length; i++) {
                let s1 = stopComponent.createObject(root, {
                    "position": root.node.gradient[i][0],
                    "color": root.node.gradient[i][1]
                });
                stopsList.push(s1);
            }
            gradient.stops = stopsList;
            gradient.y2 = root.y;
            gradient.y2 = root.y + root.height;
        }

        fillGradient: LinearGradient {
            id: gradient
            x1: root.x
            y1: root.y
            x2: root.x + root.width
            y2: root.y + root.height
        }
    }
}
