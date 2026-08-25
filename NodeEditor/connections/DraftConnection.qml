pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

Loader {
    id: draftConnection

    required property NavigableArea area
    required property NodeList nodeList
    required property AbstractContext context

    property var selectedPort: null

    active: selectedPort !== null

    function tryToCreate() {
        const oppositeCount = selectedPort.portSide === NodeEditor.PortSide.In ? NodeEditor.NodeRole.OutPortCount : NodeEditor.NodeRole.InPortCount;

        for (let i = 0; i < nodeList.nodes.count; ++i) {
            const node = nodeList.nodes.itemAt(i) as NodeObject;
            if (node.nodeId != selectedPort.nodeId) {
                const ports = context.graphModel.nodeData(node.nodeId, oppositeCount);
                if (ports < 1)
                    continue;
                for (let j = 0; j < ports; j++) {
                    const relativePortPos = node.getPortPosition(j, NodeEditorUtils.oppositeSide(selectedPort.portSide));

                    const pos = Qt.point(relativePortPos.x + node.x, relativePortPos.y + node.y);
                    const diff = Qt.vector2d(pos.x - area.mousePosition.x, pos.y - area.mousePosition.y);
                    const dist = Math.sqrt(diff.dotProduct(diff));

                    const tolerance = node.style.connectionPointDiameter * 2.0;

                    if (dist < tolerance) {
                        let newConnection;
                        if (selectedPort.portSide === NodeEditor.PortSide.In) {
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
                        if (context.graphModel.connectionPossible(newConnection)) {
                            context.graphModel.addConnection(newConnection);
                        }
                        selectedPort = null;
                        return;
                    }
                }
            }
        }
        context.graphModel.connectionDropped(selectedPort.nodeId, selectedPort.portSide, selectedPort.portId, area.mousePosition.x, area.mousePosition.y);
        selectedPort = null;
    }

    sourceComponent: ConnectionObject {
        id: painter

        context: draftConnection.context

        Component.onCompleted: {
            forceActiveFocus();
        }

        property var sp: draftConnection.selectedPort

        Keys.onEscapePressed: {
            draftConnection.selectedPort = null;
        }

        area: draftConnection.area
        nodeList: draftConnection.nodeList

        connection: ({
                outNodeId: sp.portSide === NodeEditor.PortSide.In ? NodeEditorUtils.InvalidNodeId : sp.nodeId,
                outPortIndex: sp.portSide === NodeEditor.PortSide.In ? NodeEditorUtils.InvalidPortIndex : sp.portId,
                inNodeId: sp.portSide === NodeEditor.PortSide.In ? sp.nodeId : NodeEditorUtils.InvalidNodeId,
                inPortIndex: sp.portSide === NodeEditor.PortSide.In ? sp.portId : NodeEditorUtils.InvalidPortIndex
            })

        Connections {
            target: draftConnection.area.dragArea
            function onReleased() {
                draftConnection.tryToCreate();
            }
        }
    }
}
