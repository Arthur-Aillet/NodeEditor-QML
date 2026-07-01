import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

FlexboxLayout {
    id: colorPicker
    property alias value: colorDialog.selectedColor

    direction: FlexboxLayout.Row
    alignItems: FlexboxLayout.AlignCenter
    gap: 6

    ColorDialog {
        id: colorDialog
        options: ColorDialog.ShowAlphaChannel | ColorDialog.DontUseNativeDialog | ColorDialog.NoButtons
        modality: Qt.NonModal
    }

    Rectangle {
        id: colorDisplay
        width: 35
        height: 25
        color: colorPicker.value
        radius: 4
        border.color: palette.shadow
        border.width: 1
    }
    Button {
        text: "select"
        spacing: 2
        onPressed: colorDialog.open()
    }
}
