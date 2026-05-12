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
        const oppositeCount = selectedPort.portType === NodeEditor.PortType.In ? NodeEditor.NodeRole.OutPortCount : NodeEditor.NodeRole.InPortCount;
        const oppositeSide = selectedPort.portType === NodeEditor.PortType.In ? NodeEditor.PortType.Out : NodeEditor.PortType.In;

        for (let i = 0; i < nodes.nodes.count; ++i) {
            const node = nodes.nodes.itemAt(i) as NodeGraphicalObject;
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
                        if (selectedPort.portType === NodeEditor.PortType.In)
                            ModelInterface.createConnection({
                                inNodeId: selectedPort.nodeId,
                                inPortIndex: selectedPort.portId,
                                outNodeId: node.nodeId,
                                outPortIndex: j
                            });
                        else
                            ModelInterface.createConnection({
                                inNodeId: node.nodeId,
                                inPortIndex: j,
                                outNodeId: selectedPort.nodeId,
                                outPortIndex: selectedPort.portId
                            });
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

        connection: ({
                outNodeId: selectedPort.portType === NodeEditor.PortType.In ? NodeEditorUtils.InvalidNodeId : selectedPort.nodeId,
                outPortIndex: selectedPort.portType === NodeEditor.PortType.In ? NodeEditorUtils.InvalidPortIndex : selectedPort.portId,
                inNodeId: selectedPort.portType === NodeEditor.PortType.In ? selectedPort.nodeId : NodeEditorUtils.InvalidNodeId,
                inPortIndex: selectedPort.portType === NodeEditor.PortType.In ? selectedPort.portId : NodeEditorUtils.InvalidPortIndex
            })

        Connections {
            target: root.area.dragArea
            function onReleased() {
                root.tryCreation();
            }
        }
    }
}
