pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes

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

    function letterAsData() {
        if (char == "A" || char == "a") {
            return [
                {
                    layer: 1,
                    angle: 0
                },
                {
                    layer: 2,
                    angle: 0
                },
                {
                    layer: 2,
                    angle: 360 * 1 / 3
                },
                {
                    layer: 2,
                    angle: 360 * 2 / 3
                }
            ];
        } else {
            return [
                {
                    layer: 2,
                    angle: 90
                },
                {
                    layer: 2,
                    angle: -90
                }
            ];
        }
    }

    function toAbsoluteAngle(angle: real): real {
        while (angle < 0) {
            angle += 360;
        }
        return angle % 360;
    }

    function processLayer(newCharData) {
        let indexMapping = {};
        let updateIndexes = [];
        let toAppend = [];

        for (let i = 0; i != newCharData.length; i++) {
            if (i >= layer2.length) {
                for (let j = 0; j != newCharData.length; j++) {
                    if (!(j in indexMapping)) {
                        console.log("adding " + j);
                        toAppend.push({
                            layer: 2,
                            color: "black",
                            angle: newCharData[j].angle
                        });
                    }
                }
            } else {
                let closestDistance = Infinity;
                let indexNew = -1;
                let indexOld = -1;

                for (let j = 0; j != newCharData.length; j++) {
                    if (j in indexMapping) {
                        continue;
                    }

                    for (let k = 0; k != layer2.length; k++) {
                        const distance = Math.abs(toAbsoluteAngle(newCharData[j].angle) - toAbsoluteAngle(layer2[k].angle));

                        if (distance < closestDistance) {
                            closestDistance = distance;
                            indexNew = j;
                            indexOld = k;
                        }
                    }
                }

                if (indexNew != -1) {
                    indexMapping[indexNew] = indexOld;
                }
                console.log("mapped: " + indexNew + "  " + indexOld);
                updateIndexes.push({
                    index: indexOld,
                    angle: newCharData[indexNew].angle
                });
            }
        }
        if (newCharData.length < layer2.length) {
            console.log("prunning");
        }

        layer2.concat(toAppend);

        for (let i = 0; i != updateIndexes.length; i++) {
            console.log("> " + updateIndexes[i].index + "  " + updateIndexes[i].angle);
            repeater.itemAt(updateIndexes[i].index).angle = root.toAbsoluteAngle(updateIndexes[i].angle);
        }
    }

    onCharChanged: {
        let letter = letterAsData();

        //processLayer(letter.filter(el => el.layer == 1), layer1);
        processLayer(letter.filter(el => el.layer == 2));
    }

    property var dots: layer1.concat(layer2)
    property var layer1: []
    property var layer2: [
        {
            layer: 2,
            angle: 90,
            color: "red"
        },
        {
            layer: 2,
            angle: -90,
            color: "purple"
        }
    ]

    Repeater {
        id: repeater
        model: root.layer2
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
                fillColor: dot.modelData.color

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
