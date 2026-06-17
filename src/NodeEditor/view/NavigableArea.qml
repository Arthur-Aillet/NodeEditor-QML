import QtQuick
import NodeEditor

Item {
    id: root
    anchors.fill: parent

    // Disable drag when holding an Item (Like a draft connection)
    required property bool holdingItem
    property point mousePosition

    default property alias subdata: inner.data
    property alias inner: inner
    property alias background: background
    property alias dragArea: dragArea

    ViewBackground {
        id: background
        zoom: inner.mat.m11
        translate: Qt.point(inner.mat.m14 + inner.x, inner.mat.m24 + inner.y)
    }

    // ViewBackground {
    //     id: background

    //     property real zoom: inner.mat.m11
    //     property real zoomedScale: (150 * zoom)
    //     property real offsetX: inner.mat.m14 + inner.x
    //     property real offsetY: inner.mat.m24 + inner.y

    //     x: offsetX % zoomedScale - (offsetX > 0) * zoomedScale
    //     y: offsetY % zoomedScale - (offsetY > 0) * zoomedScale
    //     antialiasing: zoom < 1
    //     smooth: zoom < 1
    //     width: (parent.width - x) / zoom + 1 // + 1 to hide any rounding error
    //     height: (parent.height - y) / zoom + 1
    //     transform: [
    //         Scale {
    //             xScale: background.zoom
    //             yScale: background.zoom
    //         }
    //     ]
    // }

    MouseArea {
        id: dragArea
        hoverEnabled: true
        anchors.fill: parent
        drag.target: root.holdingItem ? undefined : inner
        drag.filterChildren: true
        focusPolicy: Qt.WheelFocus

        onPositionChanged: root.mousePosition = mapToItem(inner, mouseX, mouseY)

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
