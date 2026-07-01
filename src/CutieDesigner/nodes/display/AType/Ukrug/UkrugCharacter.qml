pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

import CutieDesigner.Nodes.Display

Item {
    id: ukrugChar

    required property UkrugNode node
    required property var char
    required property bool goingToGetDestroyed

    Layout.preferredWidth: 0
    Layout.preferredHeight: node.fontSize

    Behavior on Layout.preferredWidth {
        id: widthAnimation
        NumberAnimation {
            duration: ukrugChar.node.animationWidthSpeed
        }
    }

    UkrugPointsList {
        id: list
    }

    property bool valid: false

    onCharChanged: {
        valid = list.assignLetter(char);
    }

    Component.onCompleted: {
        Layout.preferredWidth = Qt.binding(function () {
            return ukrugChar.node.fontSize;
        });
        colorAnimation.duration = 0;
        shader.baseColor = Qt.binding(function () {
            return ukrugChar.node.baseColor;
        });
    }

    Timer {
        id: timer
        running: false
        repeat: false

        property var callback

        onTriggered: callback()
    }

    onGoingToGetDestroyedChanged: {
        if (node.animationWidthSpeed > node.animationOpacitySpeed) {
            timer.interval = node.animationWidthSpeed - node.animationOpacitySpeed;
            Layout.preferredWidth = 0;
            timer.callback = () => {
                colorAnimation.duration = node.animationOpacitySpeed;
                shader.baseColor = "transparent";
            };
        } else {
            timer.interval = node.animationOpacitySpeed - node.animationWidthSpeed;
            colorAnimation.duration = node.animationOpacitySpeed;
            shader.baseColor = "transparent";
            timer.callback = () => {
                Layout.preferredWidth = 0;
            };
        }
        timer.running = true;
    }

    UkrugShader {
        id: shader
        anchors.fill: parent
        source: parent

        property alias node: ukrugChar.node

        baseColor: "transparent"
        Behavior on baseColor {
            PropertyAnimation {
                id: colorAnimation
                duration: ukrugChar.node.animationOpacitySpeed
            }
        }

        k: node.k
        circleScale: ukrugChar.valid ? node.circleScale : 0.1 * node.circleScale
        Behavior on circleScale {
            PropertyAnimation {
                duration: ukrugChar.node.animationTransformSpeed
            }
        }
        pointsScale: node.pointsScale
        Behavior on pointsScale {
            PropertyAnimation {
                duration: ukrugChar.node.animationTransformSpeed
            }
        }
        smoothFactor: node.smoothFactor
        fill: node.fill
        Behavior on fill {
            PropertyAnimation {
                duration: ukrugChar.node.animationTransformSpeed
            }
        }
        substraction: node.substraction
        boxArea: Qt.point((ukrugChar.Layout.preferredWidth / node.fontSize) * node.boxLimitX, node.boxLimitY)
        boxRadius: node.boxRadius

        function convertToPoint(angle, distance) {
            let vec = Qt.vector2d(Math.cos(angle), Math.sin(angle));
            vec = vec.times(distance * node.pointsDistance);
            return Qt.point(vec.x, vec.y);
        }

        property double point1angle: list.point1.angle
        property double point1distance: list.point1.distance
        point1: convertToPoint(point1angle, point1distance)

        Behavior on point1angle {
            PropertyAnimation {
                duration: list.point1.animateAngle ? ukrugChar.node.animationTransformSpeed : 0
            }
        }
        Behavior on point1distance {
            PropertyAnimation {
                duration: ukrugChar.node.animationTransformSpeed
            }
        }

        property double point2angle: list.point2.angle
        property double point2distance: list.point2.distance
        point2: convertToPoint(point2angle, point2distance)
        Behavior on point2angle {
            PropertyAnimation {
                duration: list.point2.animateAngle ? ukrugChar.node.animationTransformSpeed : 0
            }
        }
        Behavior on point2distance {
            PropertyAnimation {
                duration: ukrugChar.node.animationTransformSpeed
            }
        }

        property double point3angle: list.point3.angle
        property double point3distance: list.point3.distance
        point3: convertToPoint(point3angle, point3distance)
        Behavior on point3angle {
            PropertyAnimation {
                duration: list.point3.animateAngle ? ukrugChar.node.animationTransformSpeed : 0
            }
        }
        Behavior on point3distance {
            PropertyAnimation {
                duration: ukrugChar.node.animationTransformSpeed
            }
        }

        property double point4angle: list.point4.angle
        property double point4distance: list.point4.distance
        point4: convertToPoint(point4angle, point4distance)
        Behavior on point4angle {
            PropertyAnimation {
                duration: list.point4.animateAngle ? ukrugChar.node.animationTransformSpeed : 0
            }
        }
        Behavior on point4distance {
            PropertyAnimation {
                duration: ukrugChar.node.animationTransformSpeed
            }
        }
    }
}
