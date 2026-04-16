import QtQuick
import GraphModule

Window {
    id: root
    width: 800
    height: 800
    visible: true
    title: qsTr("CutieDesigner!")
    MyNodeEditor {
        id: qmlwrap
        width: 800
        height: 800
    }
}
