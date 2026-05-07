import QtQuick
import QtQuick.Shapes
import NodeModule

Shape {
    id: root
    property point mousePos

    required property NodeList nodes

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
    property var inNodePos: undefined

    function getNodePosition(id: int): var {
        for (let i = 0; i != nodes.nodes.count; i++) {
            const node = nodes.nodes.itemAt(i) as NodeObject;
            if (node.nodeId == id) {
                return Qt.point(node.x, node.y);
            }
        }
    }

    Binding on inNodePos {
        when: root.inNodeId !== undefined
        value: if (root.inNodeId !== undefined)
            root.getNodePosition(root.inNodeId)
    }

    property var inPortPos: inNodeId !== undefined ? ModelInterface.nodeGeometry.portPosition(inNodeId, PortType.In, inPortIndex) : undefined
    required property var outNodeId
    required property var outPortIndex
    property var outNodePos: undefined

    Binding on outNodePos {
        when: root.outNodeId !== undefined
        value: if (root.outNodeId !== undefined)
            root.getNodePosition(root.outNodeId)
    }

    property var outPortPos: outNodeId !== undefined ? ModelInterface.nodeGeometry.portPosition(outNodeId, PortType.Out, outPortIndex) : undefined

    property point inPoint: inNodeId === undefined ? mousePos : Qt.point(inPortPos.x + inNodePos.x, inPortPos.y + inNodePos.y)
    property point outPoint: outNodeId === undefined ? mousePos : Qt.point(outPortPos.x + outNodePos.x, outPortPos.y + outNodePos.y)

    property point c1
    property point c2

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
    preferredRendererType: Shape.CurveRenderer

    property bool fullyConnected: root.inNodeId !== undefined && root.outNodeId !== undefined
    property bool hovered: false

    onMousePosChanged: {
        hovered = false;
        const rect = root.boundingRect;
        if (mousePos.x < rect.x || mousePos.y < rect.y || mousePos.x > rect.x + rect.width || mousePos.y > rect.y + rect.height) {
            return;
        }
        const strokeWidth = 10;
        const overlapAndCurveFactor = 1.2;
        const steps = Math.round(Math.sqrt(rect.height * rect.height + rect.width * rect.width) / strokeWidth * overlapAndCurveFactor);
        for (let i = 0; i != steps; i++) {
            const pointAlong = path.pointAtPercent(1 / steps * i);
            const diff = Qt.vector2d(pointAlong.x - mousePos.x, pointAlong.y - mousePos.y);
            const dist = Math.sqrt(diff.dotProduct(diff));
            if (dist < strokeWidth) {
                hovered = true;
                return;
            }
        }
    }

    TapHandler {
        onTapped: {
            if (root.hovered) {
                root.focus = true;
            }
        }
    }

    Keys.onPressed: event => {
        if (!fullyConnected)
            return;

        if (event.key == Qt.Key_Delete || event.key == Qt.Key_Back) {
            ModelInterface.deleteConnection(inNodeId, inPortIndex, outNodeId, outPortIndex);
        }
    }

    ShapePath {
        id: outline

        startX: root.inPoint.x
        startY: root.inPoint.y
        fillColor: "transparent"
        strokeWidth: 4
        strokeColor: root.focus ? "orange" : (root.fullyConnected && root.hovered ? "white" : "transparent")
        strokeStyle: ShapePath.SolidLine
        PathCubic {
            x: root.outPoint.x
            y: root.outPoint.y
            control1X: root.c1.x
            control1Y: root.c1.y
            control2X: root.c2.x
            control2Y: root.c2.y
        }
    }

    ShapePath {
        id: path
        property bool swapped: root.inNodeId === undefined

        startX: swapped ? root.outPoint.x : root.inPoint.x
        startY: swapped ? root.outPoint.y : root.inPoint.y
        fillColor: "transparent"
        strokeWidth: 2
        strokeColor: root.fullyConnected ? "teal" : "grey"
        strokeStyle: root.fullyConnected ? ShapePath.SolidLine : ShapePath.DashLine
        dashPattern: [6, 2]
        PathCubic {
            x: path.swapped ? root.inPoint.x : root.outPoint.x
            y: path.swapped ? root.inPoint.y : root.outPoint.y
            control1X: path.swapped ? root.c2.x : root.c1.x
            control1Y: path.swapped ? root.c2.y : root.c1.y
            control2X: path.swapped ? root.c1.x : root.c2.x
            control2Y: path.swapped ? root.c1.y : root.c2.y
        }
    }
}
