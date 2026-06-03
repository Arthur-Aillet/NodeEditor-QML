import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FlexboxLayout {
    id: root
    direction: FlexboxLayout.Row
    alignItems: FlexboxLayout.AlignCenter
    justifyContent: FlexboxLayout.JustifyStart
    gap: 6

    property alias text: textField.text
    property alias value: textField.text
    property alias textField: textField

    TextField {
        id: textField
        Layout.preferredWidth: Math.max(40, contentWidth + 8)
    }
}
