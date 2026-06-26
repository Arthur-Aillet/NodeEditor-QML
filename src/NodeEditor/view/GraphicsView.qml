pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeEditor

Frame {
    id: root
    clip: true
    padding: 1

    property alias area: areaId

    NavigableArea {
        id: areaId
        width: root.width
        height: root.height
        holdingItem: (draftConnectionId.selectedPort !== null) || (nodesId.selectedNodes.size !== 0)

        ConnectionList {
            area: areaId
            nodes: nodesId
        }

        DraftConnection {
            id: draftConnectionId
            nodes: nodesId
            area: areaId
        }

        NodeList {
            id: nodesId
            area: areaId
            draftConnection: draftConnectionId
        }
    }

    ContextMenu.menu: SceneMenu {
        area: areaId
    }
}
