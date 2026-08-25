pragma ComponentBehavior: Bound
import QtQuick
import NodeEditor

Item {
    id: connectionList

    required property NavigableArea area
    required property NodeList nodes
    required property AbstractContext context

    Connections {
        target: connectionList.area
        function onMousePositionChanged() {
            let closestIndex = undefined;
            let closestDistance = Infinity;

            for (let i = 0; i < connections.count; i++) {
                const connection = connections.itemAt(i) as ConnectionObject;
                const dist = connection.distanceToCurve(connectionList.area.mousePosition);
                if (dist < closestDistance) {
                    closestDistance = dist;
                    closestIndex = i;
                }
                connection.hovered = false;
            }
            if (closestDistance < 10) {
                connections.itemAt(closestIndex).hovered = true;
            }
        }
    }

    Connections {
        target: connectionList.context.graphModel

        function equal(fst: connectionId, snd: connectionId): bool {
            return fst.outNodeId == snd.outNodeId && fst.outPortIndex == snd.outPortIndex && fst.inNodeId == snd.inNodeId && fst.inPortIndex == snd.inPortIndex;
        }

        function onConnectionDeleted(id) {
            for (let i = 0; i < connectionModel.count; ++i) {
                const connection = connectionModel.get(i);
                if (equal(connection.inputConnectionId, id)) {
                    connectionModel.remove(i);
                }
            }
        }

        function onConnectionCreated(id) {
            connectionModel.append({
                "inputConnectionId": id
            });
        }
    }

    Repeater {
        id: connections

        delegate: ConnectionObject {
            required property connectionId inputConnectionId

            connection: inputConnectionId
            context: connectionList.context
            nodes: connectionList.nodes
            area: connectionList.area
        }

        model: ListModel {
            id: connectionModel
        }
    }
}
