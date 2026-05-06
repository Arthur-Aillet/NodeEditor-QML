pragma ComponentBehavior: Bound
import QtQuick
import NodeModule

Item {
    id: root

    required property NavigableArea area

    Connections {
        target: ModelInterface

        function onConnectionDeleted(inNodeId, inPortIndex, outNodeId, outPortIndex) {
            for (let i = 0; i < connectionModel.count; ++i) {
                const connection = connectionModel.get(i);
                if (connection.inputInNodeId == inNodeId && connection.inputInPortIndex == inPortIndex && connection.inputOutNodeId == outNodeId && connection.inputOutPortIndex == outPortIndex)
                    connectionModel.remove(i);
            }
        }

        function onConnectionCreated(inNodeId, inPortIndex, outNodeId, outPortIndex) {
            connectionModel.append({
                "inputInNodeId": inNodeId,
                "inputInPortIndex": inPortIndex,
                "inputOutNodeId": outNodeId,
                "inputOutPortIndex": outPortIndex
            });
        }
    }

    Repeater {
        delegate: DefaultConnection {
            required property real inputInNodeId
            required property real inputInPortIndex
            required property real inputOutNodeId
            required property real inputOutPortIndex
            inNodeId: inputInNodeId
            inPortIndex: inputInPortIndex
            outNodeId: inputOutNodeId
            outPortIndex: inputOutPortIndex

            mousePos: root.area.mousePosition
        }

        model: ListModel {
            id: connectionModel
        }
    }
}
