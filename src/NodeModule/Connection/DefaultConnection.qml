import QtQuick
import QtQuick.Shapes
import NodeModule

Shape {
    id: root

    property point mousePosition

    function getPosition(node: var, port: var, type): point {
        if (node == null || port == null) {
            return mousePosition;
        }

        const portPos = ModelInterface.nodeGeometry.portPosition(node, type, port);
        const nodePos = ModelInterface.nodeData(node, NodeRole.Position);
        return Qt.point(portPos.x + nodePos.x, portPos.y + nodePos.y);
    }

    enum MouseConnectedTo {
        In,
        Out,
        None
    }

    Binding on inPoint {
        when: root.mouseState == DefaultConnection.MouseConnectedTo.In
        value: root.mousePosition
    }

    Binding on inPoint {
        when: root.mouseState != DefaultConnection.MouseConnectedTo.In
        value: root.getPosition(root.inNodeId, root.inPortIndex, PortType.In)
    }

    Binding on outPoint {
        when: root.mouseState == DefaultConnection.MouseConnectedTo.Out
        value: root.mousePosition
    }

    Binding on outPoint {
        when: root.mouseState != DefaultConnection.MouseConnectedTo.Out
        value: root.getPosition(root.outNodeId, root.outPortIndex, PortType.Out)
    }

    property int mouseState: inNodeId == null ? DefaultConnection.MouseConnectedTo.In : (outNodeId == null ? DefaultConnection.MouseConnectedTo.Out : DefaultConnection.MouseConnectedTo.None)

    //undefined or int
    required property var inNodeId
    required property var inPortIndex
    required property var outNodeId
    required property var outPortIndex

    property point inPoint
    property point outPoint

    property point c1
    property point c2

    x: Math.min(inPoint.x, outPoint.x, c1.x, c2.x)
    y: Math.min(inPoint.y, outPoint.y, c1.y, c2.y)

    property bool horizontal: true
    readonly property real defaultOffset: 200

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
    antialiasing: true
    smooth: true

    ShapePath {
        id: path
        startX: root.outPoint.x - root.x
        startY: root.outPoint.y - root.y
        fillColor: "transparent"
        strokeWidth: 2
        strokeColor: "grey"
        strokeStyle: ShapePath.DashLine
        dashPattern: [6, 2]
        PathCubic {
            x: root.inPoint.x - root.x
            y: root.inPoint.y - root.y
            control1X: root.c1.x - root.x
            control1Y: root.c1.y - root.y
            control2X: root.c2.x - root.x
            control2Y: root.c2.y - root.y
        }
    }
}
