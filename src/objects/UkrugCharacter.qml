pragma ComponentBehavior: Bound
import QtQuick

import CutieDesignerModule

Item {
    id: root
    width: 150
    height: 150

    required property var char
    property bool firstCall: true

    UkrugPointsList {
        id: list
    }

    onCharChanged: {
        list.assignLetter(char);
    }

    Ukrug {
        id: shader
        anchors.fill: parent
        source: parent
        k: 0.02
        scale: 0.22
        smoothFactor: 0.01

        function convertToPoint(angle, distance) {
            let vec = Qt.vector2d(Math.cos(angle), Math.sin(angle));
            vec = vec.times(distance * 0.45);
            return Qt.point(vec.x, vec.y);
        }

        property double point1angle: list.point1.angle
        property double point1distance: list.point1.distance
        point1: convertToPoint(point1angle, point1distance)
        Behavior on point1angle {
            PropertyAnimation {
                duration: list.point1.animateAngle ? 250 : 0
            }
        }
        Behavior on point1distance {
            PropertyAnimation {}
        }

        property double point2angle: list.point2.angle
        property double point2distance: list.point2.distance
        point2: convertToPoint(point2angle, point2distance)
        Behavior on point2angle {
            PropertyAnimation {
                duration: list.point2.animateAngle ? 250 : 0
            }
        }
        Behavior on point2distance {
            PropertyAnimation {}
        }

        property double point3angle: list.point3.angle
        property double point3distance: list.point3.distance
        point3: convertToPoint(point3angle, point3distance)
        Behavior on point3angle {
            PropertyAnimation {
                duration: list.point3.animateAngle ? 250 : 0
            }
        }
        Behavior on point3distance {
            PropertyAnimation {}
        }

        property double point4angle: list.point4.angle
        property double point4distance: list.point4.distance
        point4: convertToPoint(point4angle, point4distance)
        Behavior on point4angle {
            PropertyAnimation {
                duration: list.point4.animateAngle ? 250 : 0
            }
        }
        Behavior on point4distance {
            PropertyAnimation {}
        }
    }
}
