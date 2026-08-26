import QtQuick
import NodeEditor

Item {
    id: navigableArea
    anchors.fill: parent

    // Disable drag when holding an Item (Like a draft connection)
    required property bool holdingItem
    property point mousePosition

    default property alias subdata: inner.data
    property alias inner: inner
    property alias background: background
    property alias dragArea: dragArea
    property graphicsViewStyle style

    ViewBackground {
        id: background
        zoom: inner.mat.m11
        translate: Qt.point(inner.mat.m14 + inner.x, inner.mat.m24 + inner.y)
        backgroundColor: navigableArea.style.backgroundColor
        fineGridColor: navigableArea.style.fineGridColor
        coarseGridColor: navigableArea.style.coarseGridColor
    }

    MouseArea {
        id: dragArea
        hoverEnabled: true
        anchors.fill: parent
        drag.target: navigableArea.holdingItem ? undefined : inner
        drag.filterChildren: true
        focusPolicy: Qt.ClickFocus

        onPositionChanged: navigableArea.mousePosition = mapToItem(inner, mouseX, mouseY)

        property real zoomMax: 2
        property real zoomStep: 0.03
        property real zoomMin: 0.3

        onWheel: wheel => {
            if (wheel.angleDelta.y == 0)
                return;

            const mapped = dragArea.mapToItem(inner, mouseX, mouseY);

            let currentZoom = inner.mat.m11;
            if (wheel.angleDelta.y > 0 && (currentZoom + zoomStep < zoomMax)) {
                currentZoom += zoomStep;
            } else if (wheel.angleDelta.y < 0 && (currentZoom - zoomStep > zoomMin)) {
                currentZoom -= zoomStep;
            }
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
