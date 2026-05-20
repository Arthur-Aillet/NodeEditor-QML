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

    // function processLayer(newCharData) {
    //     let indexMapping = {};
    //     let updateIndexes = [];
    //     let toAppend = [];

    //     for (let i = 0; i != newCharData.length; i++) {
    //         if (i >= repeater.count) {
    //             for (let j = 0; j != newCharData.length; j++) {
    //                 if (!(j in toAppend)) {
    //                     console.log("To append " + j);
    //                     toAppend.push({
    //                         layer: 2,
    //                         color: "black",
    //                         angle: newCharData[j].angle
    //                     });
    //                 }
    //             }
    //         } else {
    //             let closestDistance = Infinity;
    //             let indexNew = -1;
    //             let indexOld = -1;

    //             for (let j = 0; j != newCharData.length; j++) {
    //                 if (j in indexMapping) {
    //                     continue;
    //                 }

    //                 for (let k = 0; k != repeater.count; k++) {
    //                     const distance = Math.abs(toAbsoluteAngle(newCharData[j].angle) - toAbsoluteAngle(repeater.itemAt(k).angle));

    //                     if (distance < closestDistance) {
    //                         closestDistance = distance;
    //                         indexNew = j;
    //                         indexOld = k;
    //                     }
    //                 }
    //             }

    //             if (indexNew != -1) {
    //                 indexMapping[indexNew] = indexOld;
    //             }
    //             console.log("mapped: " + indexNew + "  " + indexOld);
    //             updateIndexes.push({
    //                 index: indexOld,
    //                 angle: newCharData[indexNew].angle
    //             });
    //         }
    //     }

    //     // prunning
    //     for (let i = repeater.count - newCharData.length; i > 0; i--) {
    //         console.log("prunning " + i);

    //         for (let j = 0; j != repeater.count; j++) {
    //             console.log("mapping: " + Object.values(indexMapping));
    //             if (j in Object.values(indexMapping)) {
    //                 continue;
    //             }
    //             console.log("J is free" + j);
    //         }
    //     }

    //     repeater.model = repeater.model.concat(toAppend);

    //     for (let i = 0; i != updateIndexes.length; i++) {
    //         console.log("> " + updateIndexes[i].index + "  " + updateIndexes[i].angle);
    //         //repeater.itemAt(updateIndexes[i].index).angle = root.toAbsoluteAngle(updateIndexes[i].angle);
    //     }
    // }

    onCharChanged: {
        pointsModel.setLetter(char);
        pointsModel.print();

        //processLayer(letter.filter(el => el.layer == 1), layer1);
        //processLayer(letter.filter(el => el.layer == 2));
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
