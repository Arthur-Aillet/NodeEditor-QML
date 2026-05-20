pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

FlexboxLayout {
    id: root
    property string text: "A"

    anchors.fill: parent
    direction: FlexboxLayout.Row
    justifyContent: FlexboxLayout.JustifyStart
    gap: 15

    Text {
        color: "white"
        text: root.text
        font.bold: true
        font.pixelSize: 80
    }

    // Repeater {
    //     id: rep
    //     model: root.text.length
    //     delegate: UkrugCharacter {
    //         required property int index
    //         char: root.text[index]
    //     }
    // }
}
