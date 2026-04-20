import QtQuick
import NodeModule

Item {
    id: root
    clip: true
    anchors.fill: parent

    ViewBackground {
        id: view
        property real realScale: 150
        property real zoom: area.mat.m11
        property real zoomedScale: (150 * zoom)
        property real offsetX: area.mat.m14 + area.x
        property real offsetY: area.mat.m24 + area.y
        property real realtiveX: offsetX % zoomedScale - (offsetX > 0) * zoomedScale
        property real realtiveY: offsetY % zoomedScale - (offsetY > 0) * zoomedScale

        x: realtiveX
        y: realtiveY
        antialiasing: zoom < 1
        smooth: zoom < 1
        width: (parent.width - realtiveX) / zoom + 1
        height: (parent.height - realtiveY) / zoom + 1
        transform: [
            Scale {
                xScale: view.zoom
                yScale: view.zoom
            }
        ]
    }

    Item {
        id: area
        width: parent.width
        height: parent.height
        property matrix4x4 mat: Qt.matrix4x4()

        transform: [
            Matrix4x4 {
                id: scaler
                matrix: area.mat
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

        property real zoomMax: 2
        property real zoomStep: 0.03
        property real zoomMin: 0.3

        onWheel: wheel => {
            const mapped = dragArea.mapToItem(area, mouseX, mouseY);

            var currentZoom = area.mat.m11;
            if (wheel.angleDelta.y > 0) {
                currentZoom += zoomStep;
            } else {
                currentZoom -= zoomStep;
            }
            currentZoom = Math.max(Math.min(currentZoom, zoomMax), zoomMin);
            area.mat.translate(Qt.vector2d(mapped.x, mapped.y));
            area.mat.m11 = currentZoom;
            area.mat.m22 = currentZoom;
            area.mat.translate(Qt.vector2d(-mapped.x, -mapped.y));
        }
    }
}
