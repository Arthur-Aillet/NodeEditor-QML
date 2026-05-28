pragma ComponentBehavior: Bound
import QtQuick
import NodeModule

Loader {
    id: root

    required property NavigableArea area
    required property NodeList nodes

    property var selectedPort: null

    active: selectedPort !== null

    function tryToCreate() {
        const oppositeCount = selectedPort.portType === NodeEditor.PortType.In ? NodeEditor.NodeRole.OutPortCount : NodeEditor.NodeRole.InPortCount;
        const oppositeSide = selectedPort.portType === NodeEditor.PortType.In ? NodeEditor.PortType.Out : NodeEditor.PortType.In;

        for (let i = 0; i < nodes.nodes.count; ++i) {
            const node = nodes.nodes.itemAt(i) as NodeGraphicalObject;
            if (node.nodeId != selectedPort.nodeId) {
                const ports = ModelInterface.nodeData(node.nodeId, oppositeCount);
                if (ports < 1)
                    continue;
                for (let j = 0; j < ports; j++) {
                    const relativePortPos = node.getPortPosition(j, oppositeSide);

                    const pos = Qt.point(relativePortPos.x + node.x, relativePortPos.y + node.y);
                    const diff = Qt.vector2d(pos.x - area.mousePosition.x, pos.y - area.mousePosition.y);
                    const dist = Math.sqrt(diff.dotProduct(diff));

                    const tolerance = node.style.connectionPointDiameter * 2.0;

                    if (dist < tolerance) {
                        let newConnection;
                        if (selectedPort.portType === NodeEditor.PortType.In) {
                            newConnection = {
                                inNodeId: selectedPort.nodeId,
                                inPortIndex: selectedPort.portId,
                                outNodeId: node.nodeId,
                                outPortIndex: j
                            };
                        } else {
                            newConnection = {
                                inNodeId: node.nodeId,
                                inPortIndex: j,
                                outNodeId: selectedPort.nodeId,
                                outPortIndex: selectedPort.portId
                            };
                        }
                        if (ModelInterface.graph.connectionPossible(newConnection)) {
                            ModelInterface.createConnection(newConnection);
                        }
                        selectedPort = null;
                        return;
                    }
                }
            }
        }
        selectedPort = null;
    }

    // TODO: Default Connection will also become a ConnectionObject + ConnectionPainter, just like Nodes
    sourceComponent: DefaultConnection {
        id: painter

        property var p: root.selectedPort

        area: root.area
        nodes: root.nodes

        connection: ({
                outNodeId: p.portType === NodeEditor.PortType.In ? NodeEditorUtils.InvalidNodeId : p.nodeId,
                outPortIndex: p.portType === NodeEditor.PortType.In ? NodeEditorUtils.InvalidPortIndex : p.portId,
                inNodeId: p.portType === NodeEditor.PortType.In ? p.nodeId : NodeEditorUtils.InvalidNodeId,
                inPortIndex: p.portType === NodeEditor.PortType.In ? p.portId : NodeEditorUtils.InvalidPortIndex
            })

        Connections {
            target: root.area.dragArea
            function onReleased() {
                root.tryToCreate();
            }
        }
    }
}
