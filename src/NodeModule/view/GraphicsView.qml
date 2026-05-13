pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeModule

Frame {
    id: root
    clip: true
    padding: 1

    property alias area: areaId

    NavigableArea {
        id: areaId
        width: root.width
        height: root.height
        holdingItem: draftConnectionId.selectedPort !== null

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
