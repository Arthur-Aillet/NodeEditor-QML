pragma ComponentBehavior: Bound
import QtQuick
import NodeModule

Loader {
    id: root

    required property NavigableArea area
    required property NodeList nodes

    property var selectedPort: null

    active: selectedPort !== null

    function tryCreation() {
        const oppositeCount = selectedPort.portType === PortType.In ? NodeRole.OutPortCount : NodeRole.InPortCount;
        const oppositeSide = selectedPort.portType === PortType.In ? PortType.Out : PortType.In;

        for (let i = 0; i < nodes.nodes.count; ++i) {
            const node = nodes.nodes.itemAt(i) as NodeObject;
            if (node.nodeId != selectedPort.nodeId) {
                const ports = ModelInterface.nodeData(node.nodeId, oppositeCount);
                if (ports < 1)
                    continue;
                for (let j = 0; j < ports; j++) {
                    const relativePortPos = ModelInterface.nodeGeometry.portPosition(node.nodeId, oppositeSide, j);

                    const pos = Qt.point(relativePortPos.x + node.x, relativePortPos.y + node.y);
                    const diff = Qt.vector2d(pos.x - area.mousePosition.x, pos.y - area.mousePosition.y);
                    const dist = Math.sqrt(diff.dotProduct(diff));

                    const style = node.style;
                    const tolerance = style.connectionPointDiameter * 2.0;

                    if (dist < tolerance) {
                        if (selectedPort.portType === PortType.In)
                            ModelInterface.createConnection(selectedPort.nodeId, selectedPort.portId, node.nodeId, j);
                        else
                            ModelInterface.createConnection(node.nodeId, j, selectedPort.nodeId, selectedPort.portId);
                        selectedPort = null;
                        return;
                    }
                }
            }
        }
        selectedPort = null;
    }

    sourceComponent: DefaultConnection {
        id: painter

        property var selectedPort: root.selectedPort

        mousePos: root.area.mousePosition
        nodes: root.nodes

        inNodeId: selectedPort.portType === PortType.In ? selectedPort.nodeId : undefined
        inPortIndex: selectedPort.portType === PortType.In ? selectedPort.portId : undefined
        outNodeId: selectedPort.portType === PortType.In ? undefined : selectedPort.nodeId
        outPortIndex: selectedPort.portType === PortType.In ? undefined : selectedPort.portId

        Connections {
            target: root.area.dragArea
            function onReleased() {
                root.tryCreation();
            }
        }
    }
}
