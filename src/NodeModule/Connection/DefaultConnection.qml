import QtQuick
import QtQuick.Shapes
import NodeModule

Shape {
    id: root
    focus: true
    property point mousePosition

    Connections {
        target: ModelInterface

        function onNodePositionUpdated(id: real) {
            if (id == root.inNodeId) {
                root.inNodePos = ModelInterface.nodeData(id, NodeRole.Position);
            }
            if (id == root.outNodeId) {
                root.outNodePos = ModelInterface.nodeData(id, NodeRole.Position);
            }
        }
    }

    //undefined or int
    required property var inNodeId
    required property var inPortIndex
    property var inNodePos: inNodeId !== undefined ? ModelInterface.nodeData(inNodeId, NodeRole.Position) : undefined
    property var inPortPos: inNodeId !== undefined ? ModelInterface.nodeGeometry.portPosition(inNodeId, PortType.In, inPortIndex) : undefined
    required property var outNodeId
    required property var outPortIndex
    property var outNodePos: outNodeId !== undefined ? ModelInterface.nodeData(outNodeId, NodeRole.Position) : undefined
    property var outPortPos: outNodeId !== undefined ? ModelInterface.nodeGeometry.portPosition(outNodeId, PortType.Out, outPortIndex) : undefined

    property point inPoint: inNodeId === undefined ? root.mousePosition : Qt.point(inPortPos.x + inNodePos.x, inPortPos.y + inNodePos.y)
    property point outPoint: outNodeId === undefined ? root.mousePosition : Qt.point(outPortPos.x + outNodePos.x, outPortPos.y + outNodePos.y)

    property point c1
    property point c2

    x: Math.min(inPoint.x, outPoint.x, c1.x, c2.x)
    y: Math.min(inPoint.y, outPoint.y, c1.y, c2.y)

    readonly property real defaultOffset: 200

    property bool horizontal: true
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
        c1 = Qt.point(inPoint.x - horizontalOffset, inPoint.y - verticalOffset);
        c2 = Qt.point(outPoint.x + horizontalOffset, outPoint.y + verticalOffset);
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
        c1 = Qt.point(inPoint.x - horizontalOffset, inPoint.y - verticalOffset);
        c2 = Qt.point(outPoint.x + horizontalOffset, outPoint.y + verticalOffset);
    }
    antialiasing: true
    smooth: true

    ShapePath {
        id: path
        property bool swapped: root.inNodeId === undefined
        startX: (swapped ? root.outPoint.x : root.inPoint.x) - root.x
        startY: (swapped ? root.outPoint.y : root.inPoint.y) - root.y
        fillColor: "transparent"
        strokeWidth: 2
        strokeColor: "grey"
        strokeStyle: ShapePath.DashLine
        dashPattern: [6, 2]
        PathCubic {
            x: (path.swapped ? root.inPoint.x : root.outPoint.x) - root.x
            y: (path.swapped ? root.inPoint.y : root.outPoint.y) - root.y
            control1X: (path.swapped ? root.c2.x : root.c1.x) - root.x
            control1Y: (path.swapped ? root.c2.y : root.c1.y) - root.y
            control2X: (path.swapped ? root.c1.x : root.c2.x) - root.x
            control2Y: (path.swapped ? root.c1.y : root.c2.y) - root.y
        }
    }
}
