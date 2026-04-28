import QtQuick
import NodeModule

Item {
    id: root
    clip: true
    anchors.fill: parent

    default property alias subdata: inner.data
    property alias inner: inner

    ViewBackground {
        id: background
        property real zoom: inner.mat.m11
        property real zoomedScale: (150 * zoom)
        property real offsetX: inner.mat.m14 + inner.x
        property real offsetY: inner.mat.m24 + inner.y
        property real realtiveX: offsetX % zoomedScale - (offsetX > 0) * zoomedScale
        property real realtiveY: offsetY % zoomedScale - (offsetY > 0) * zoomedScale

        x: realtiveX
        y: realtiveY
        antialiasing: zoom < 1
        smooth: zoom < 1
        width: (parent.width - realtiveX) / zoom + 1 // + 1 to hide any rounding error
        height: (parent.height - realtiveY) / zoom + 1
        transform: [
            Scale {
                xScale: background.zoom
                yScale: background.zoom
            }
        ]
    }

    MouseArea {
        id: dragArea
        hoverEnabled: true
        anchors.fill: parent
        drag.target: inner
        drag.filterChildren: true
        propagateComposedEvents: true
        focusPolicy: Qt.WheelFocus

        property real zoomMax: 2
        property real zoomStep: 0.03
        property real zoomMin: 0.3

        onWheel: wheel => {
            const mapped = dragArea.mapToItem(inner, mouseX, mouseY);

            var currentZoom = inner.mat.m11;
            if (wheel.angleDelta.y > 0) {
                currentZoom += zoomStep;
            } else {
                currentZoom -= zoomStep;
            }
            currentZoom = Math.max(Math.min(currentZoom, zoomMax), zoomMin);
            inner.mat.translate(Qt.vector2d(mapped.x, mapped.y));
            inner.mat.m11 = currentZoom;
            inner.mat.m22 = currentZoom;
            inner.mat.translate(Qt.vector2d(-mapped.x, -mapped.y));
        }

        Item {
            id: inner
            width: parent.width
            height: parent.height
            property matrix4x4 mat: Qt.matrix4x4()

            transform: [
                Matrix4x4 {
                    id: scaler
                    matrix: inner.mat
                }
            ]
        }
    }
}
