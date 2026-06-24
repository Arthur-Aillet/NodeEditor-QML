import QtQuick
import QtQuick.Layouts
import NodeEditor

FlexboxLayout {
    property alias graphicsView: graphicsView

    direction: FlexboxLayout.FlexboxDirection.Column
    GraphicsView {
        id: graphicsView
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
    Timeline {
        Layout.fillWidth: true
    }
}
