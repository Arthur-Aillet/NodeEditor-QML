import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import NodeEditor

MouseArea {
    property alias graphicsView: graphicsView

    hoverEnabled: true
    propagateComposedEvents: true

    Layout.fillWidth: true
    Layout.fillHeight: true

    Keys.onPressed: event => {
        if (event.key == Qt.Key_N) {
            search.x = mouseX;
            search.y = mouseY;
            search.open();
        }
    }

    GraphicsView {
        id: graphicsView
        height: parent.height - timeline.height
        anchors.left: parent.left
        anchors.right: parent.right

        ContextMenu.menu: NodeListMenu {
            area: graphicsView.area
            searchMenu: search
        }
        NodeSearchMenu {
            id: search
            area: graphicsView.area
        }
    }

    Timeline {
        id: timeline
        anchors.top: graphicsView.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
