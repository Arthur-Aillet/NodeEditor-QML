import QtQuick
import QtQuick.Controls
import NodeEditor

Frame {
    id: view
    clip: true
    padding: 1

    required property AbstractContext context

    property alias area: navigableArea
    property alias nodes: nodeListObject.nodes

    Keys.forwardTo: [nodeListObject]

    NavigableArea {
        id: navigableArea
        anchors.fill: parent
        holdingItem: (draftConnectionId.selectedPort !== null) || (nodeListObject.selectedNodes.size !== 0)
        style: view.context.styles.graphicsView

        ConnectionList {
            area: navigableArea
            nodeList: nodeListObject
            context: view.context
        }

        DraftConnection {
            id: draftConnectionId
            nodeList: nodeListObject
            area: navigableArea
            context: view.context
        }

        NodeList {
            id: nodeListObject
            area: navigableArea
            draftConnection: draftConnectionId
            context: view.context
        }
    }
}
