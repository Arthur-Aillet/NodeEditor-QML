import QtQuick
import QtQuick.Controls
import NodeEditor

Frame {
    id: view
    clip: true
    padding: 1

    required property NodeEditorContext context

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
            context: view.context
        }

        DraftConnection {
            id: draftConnectionId
            nodes: nodeList
            area: navigableArea
            context: view.context
        }

        NodeList {
            id: nodeList
            area: navigableArea
            draftConnection: draftConnectionId
            context: view.context
        }
    }
}
