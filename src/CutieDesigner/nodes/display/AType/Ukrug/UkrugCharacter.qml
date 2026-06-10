pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

import CutieDesigner

Item {
    id: root
    Layout.preferredWidth: 0
    Layout.preferredHeight: node.fontSize

    Behavior on Layout.preferredWidth {
        id: widthAnimation
        NumberAnimation {
            duration: root.node.animationWidthSpeed
        }
    }

    required property var char
    required property bool goingToGetDestroyed
    required property UkrugNode node

    UkrugPointsList {
        id: list
    }

    property bool valid: false

    onCharChanged: {
        valid = list.assignLetter(char);
    }

    Component.onCompleted: {
        Layout.preferredWidth = Qt.binding(function () {
            return root.node.fontSize;
        });
        shader.baseColor = Qt.binding(function () {
            return root.node.baseColor;
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
                shader.baseColor = "transparent";
            };
        } else {
            timer.interval = node.animationOpacitySpeed - node.animationWidthSpeed;
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

        baseColor: "transparent"
        Behavior on baseColor {
            PropertyAnimation {
                duration: root.node.animationOpacitySpeed
            }
        }

        k: root.node.k
        circleScale: root.valid ? root.node.circleScale : 0.1 * root.node.circleScale
        Behavior on circleScale {
            PropertyAnimation {
                duration: root.node.animationTransformSpeed
            }
        }
        pointsScale: root.node.pointsScale
        Behavior on pointsScale {
            PropertyAnimation {
                duration: root.node.animationTransformSpeed
            }
        }
        smoothFactor: root.node.smoothFactor
        fill: root.node.fill
        Behavior on fill {
            PropertyAnimation {
                duration: root.node.animationTransformSpeed
            }
        }
        substraction: root.node.substraction
        boxArea: Qt.point((root.Layout.preferredWidth / root.node.fontSize) * root.node.boxLimitX, root.node.boxLimitY)
        boxRadius: root.node.boxRadius
        function convertToPoint(angle, distance) {
            let vec = Qt.vector2d(Math.cos(angle), Math.sin(angle));
            vec = vec.times(distance * root.node.pointsDistance);
            return Qt.point(vec.x, vec.y);
        }

        property double point1angle: list.point1.angle
        property double point1distance: list.point1.distance
        point1: convertToPoint(point1angle, point1distance)
        Behavior on point1angle {
            PropertyAnimation {
                duration: list.point1.animateAngle ? root.node.animationTransformSpeed : 0
            }
        }
        Behavior on point1distance {
            PropertyAnimation {
                duration: root.node.animationTransformSpeed
            }
        }

        property double point2angle: list.point2.angle
        property double point2distance: list.point2.distance
        point2: convertToPoint(point2angle, point2distance)
        Behavior on point2angle {
            PropertyAnimation {
                duration: list.point2.animateAngle ? root.node.animationTransformSpeed : 0
            }
        }
        Behavior on point2distance {
            PropertyAnimation {
                duration: root.node.animationTransformSpeed
            }
        }

        property double point3angle: list.point3.angle
        property double point3distance: list.point3.distance
        point3: convertToPoint(point3angle, point3distance)
        Behavior on point3angle {
            PropertyAnimation {
                duration: list.point3.animateAngle ? root.node.animationTransformSpeed : 0
            }
        }
        Behavior on point3distance {
            PropertyAnimation {
                duration: root.node.animationTransformSpeed
            }
        }

        property double point4angle: list.point4.angle
        property double point4distance: list.point4.distance
        point4: convertToPoint(point4angle, point4distance)
        Behavior on point4angle {
            PropertyAnimation {
                duration: list.point4.animateAngle ? root.node.animationTransformSpeed : 0
            }
        }
        Behavior on point4distance {
            PropertyAnimation {
                duration: root.node.animationTransformSpeed
            }
        }
    }
}
