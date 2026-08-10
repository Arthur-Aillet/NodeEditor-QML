import QtQuick
import QtQuick.Controls
import NodeEditor

Frame {
    clip: true
    padding: 1

    property alias area: navigableArea
    property alias nodes: nodeList.nodes

    Keys.forwardTo: [nodeList]

    NavigableArea {
        id: navigableArea
        anchors.fill: parent
        holdingItem: (draftConnectionId.selectedPort !== null) || (nodeList.selectedNodes.size !== 0)

        ConnectionList {
            area: navigableArea
            nodes: nodeList
        }

        DraftConnection {
            id: draftConnectionId
            nodes: nodeList
            area: navigableArea
        }

        NodeList {
            id: nodeList
            area: navigableArea
            draftConnection: draftConnectionId
        }
    }
}
