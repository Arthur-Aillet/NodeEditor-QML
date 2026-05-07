pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeModule

Frame {
    id: root
    clip: true
    padding: 1

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
        id: menu
        onCreateNode: name => {
            ModelInterface.createNode(name, areaId.inner.mapFromItem(root, Qt.point(x, y)));
        }
    }
}
