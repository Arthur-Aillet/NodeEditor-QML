pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent
    anchors.centerIn: parent
    width: rep.width
    height: 120

    property string text: "A"

    FlexboxLayout {
        anchors.fill: parent
        direction: FlexboxLayout.Row
        justifyContent: FlexboxLayout.JustifyCenter
        gap: 15

        Repeater {
            id: rep
            model: root.text.length
            delegate: UkrugCharacter {
                required property int index
                char: root.text[index]
            }
        }
    }
}
