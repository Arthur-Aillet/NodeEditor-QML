pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes

import CutieDesignerModule

Shape {
    id: root
    width: 120
    height: 120

    required property var char

    preferredRendererType: Shape.CurveRenderer

    ShapePath {
        fillColor: "transparent"
        strokeColor: "white"
        strokeWidth: 5

        PathAngleArc {
            centerX: 60
            centerY: 60
            radiusX: 60
            radiusY: 60
            startAngle: -180
            sweepAngle: 360
        }
    }

    function toAbsoluteAngle(angle: real): real {
        while (angle < 0) {
            angle += 360;
        }
        return angle % 360;
    }

    onCharChanged: {
        pointsModel.setLetter(char);
    }

    UkrugPointsListModel {
        id: pointsModel
    }

    Repeater {
        id: repeater
        model: pointsModel
        delegate: Shape {
            id: dot
            required property var modelData
            property real angle: root.toAbsoluteAngle(modelData.angle)

            property real offset: modelData.layer == 1 ? 15 : 45

            x: 60
            y: 60 - offset

            height: 17
            width: 17

            transform: [
                Rotation {
                    origin.x: 0
                    origin.y: dot.offset
                    angle: dot.angle
                }
            ]

            Behavior on angle {
                PropertyAnimation {}
            }

            preferredRendererType: Shape.CurveRenderer
            antialiasing: true

            ShapePath {
                fillColor: "white"

                PathAngleArc {
                    centerX: 0
                    centerY: 0
                    radiusX: 8.5
                    radiusY: 8.5
                    startAngle: -180
                    sweepAngle: 360
                }
            }
        }
    }
}
