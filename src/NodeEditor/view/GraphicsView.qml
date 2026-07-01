import QtQuick
import QtQuick.Controls
import NodeEditor

Frame {
    id: root
    clip: true
    padding: 1

    property alias area: navigableArea

    NavigableArea {
        id: navigableArea
        width: root.width
        height: root.height
        holdingItem: (draftConnectionId.selectedPort !== null) || (nodesId.selectedNodes.size !== 0)

        ConnectionList {
            area: navigableArea
            nodes: nodesId
        }

        DraftConnection {
            id: draftConnectionId
            nodes: nodesId
            area: navigableArea
        }

        NodeList {
            id: nodesId
            area: navigableArea
            draftConnection: draftConnectionId
        }
    }
}
