import QtQuick
import QtQuick.Controls

import NodeEditor

ApplicationWindow {
    id: app
    width: 600
    height: 600
    visible: true
    color: "black"

    required property DataFlowContext dataFlowContext

    DefaultControlsView {
        anchors.fill: parent
        dataFlowContext: app.dataFlowContext
    }
}
