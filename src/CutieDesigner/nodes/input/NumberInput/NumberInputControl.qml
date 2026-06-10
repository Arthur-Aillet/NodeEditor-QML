import QtQuick
import QtQuick.Controls

TextField {
    id: textField
    width: Math.max(40, contentWidth + 8)
    validator: DoubleValidator {}
}
