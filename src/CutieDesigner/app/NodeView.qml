import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import NodeEditor

FlexboxLayout {
    property alias graphicsView: graphicsView

    direction: FlexboxLayout.FlexboxDirection.Column

    GraphicsView {
        id: graphicsView
        Layout.fillHeight: true
        Layout.fillWidth: true

        ContextMenu.menu: NodeListMenu {
            area: graphicsView.area
        }
    }

    Timeline {
        Layout.fillWidth: true
    }
}
