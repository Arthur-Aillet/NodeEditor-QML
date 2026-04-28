import QtQuick
import QtQuick.Shapes 1.11
import NodeModule

Shape {
    id: root

    required property point inPoint;
    required property point outPoint;
    required property real connectionId;

    property point c1
    property point c2

    property bool editing: true;
    property bool horizontal: true;
    readonly property real defaultOffset: 200

    x: outPoint.x
    y: outPoint.y

    onInPointChanged: horizontal ? pointsC1C2Horizontal() : pointsC1C2Vertical()
    onOutPointChanged: horizontal ? pointsC1C2Horizontal() : pointsC1C2Vertical()

    function pointsC1C2Horizontal() {
        const xDistance = inPoint.x - outPoint.x;
        let horizontalOffset = Math.min(defaultOffset, Math.abs(xDistance));
        let verticalOffset = 0;
        let ratioX = 0.5;
        if (xDistance <= 0) {
            let yDistance = inPoint.y - outPoint.y + 20;
            let vector = yDistance < 0 ? -1.0 : 1.0;
            verticalOffset = Math.min(defaultOffset, Math.abs(yDistance)) * vector;
            ratioX = 1.0;
        }
        horizontalOffset *= ratioX;
        c1 = Qt.point(outPoint.x + horizontalOffset, outPoint.y + verticalOffset);
        c2 = Qt.point(inPoint.x - horizontalOffset, inPoint.y - verticalOffset);
    }

    function pointsC1C2Vertical() {
        const yDistance = inPoint.y - outPoint.y;
        let verticalOffset = Math.min(defaultOffset, Math.abs(yDistance));
        let horizontalOffset = 0;
        let ratioY = 0.5;
        if (yDistance <= 0) {
            let xDistance = inPoint.x - outPoint.x + 20;
            let vector = xDistance < 0 ? -1.0 : 1.0;
            horizontalOffset = Math.min(defaultOffset, Math.abs(xDistance)) * vector;
            ratioY = 1.0;
        }
        verticalOffset *= ratioY;
        c1 = Qt.point(outPoint.x + horizontalOffset, outPoint.y + verticalOffset);
        c2 = Qt.point(inPoint.x - horizontalOffset, inPoint.y - verticalOffset);
    }

    ShapePath {
        fillColor: "transparent" 
        strokeWidth: 3
        strokeColor: "red"
        PathCubic {
            x: root.inPoint.x - root.outPoint.x
            y: root.inPoint.y - root.outPoint.y
            control1X: root.c1.x - root.outPoint.x; control1Y: root.c1.y - root.outPoint.y
            control2X: root.c2.x - root.outPoint.x; control2Y: root.c2.y - root.outPoint.y
        }
    }
}