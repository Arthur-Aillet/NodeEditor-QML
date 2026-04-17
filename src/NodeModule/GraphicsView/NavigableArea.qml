import QtQuick
import NodeModule

Rectangle {
    id: root
    clip: true
    anchors.fill: parent
    color: "red"

    ViewBackground {
        id: area
        anchors.fill: parent
        property matrix4x4 mat: Qt.matrix4x4

        transform: [
            Matrix4x4 {
                id: scaler
                matrix: area.mat
            },
            Translate {
                id: dragging
            }
        ]

        Rectangle {
            color: "blue"
            width: 100
            height: 100
            x: 30
            y: 50
        }
    }

    MouseArea {
        id: dragArea
        hoverEnabled: true
        anchors.fill: parent
        drag.target: area

        property real zoomMax: 3
        property real zoomMin: 0.5

        onWheel: {
            const mapped = dragArea.mapToItem(area, mouseX, mouseY);

            var currentZoom = area.mat.m11;
            if (wheel.angleDelta.y > 0) {
                currentZoom += 0.03;
            } else {
                currentZoom -= 0.03;
            }
            currentZoom = Math.max(Math.min(currentZoom, zoomMax), zoomMin);
            area.mat.translate(Qt.vector2d(mapped.x, mapped.y));
            area.mat.m11 = currentZoom;
            area.mat.m22 = currentZoom;
            area.mat.translate(Qt.vector2d(-mapped.x, -mapped.y));
        }
    }
}
