pragma ComponentBehavior: Bound
import QtQuick

import CutieDesigner.App
import CutieUiModule

CutieWindow {
    width: 1400
    height: 1000
    visible: true
    color: "black"
    title: qsTr("CutieDesigner!")

    property alias objectLoader: appLayout.objectLoader

    AppLayout {
        id: appLayout
    }
}
