import QtQuick
import QtQuick.Controls
import examples.calculator

TextField {
    id: inputControl

    required property DisplayNode node

    validator: DoubleValidator {}
    width: Math.max(40, contentWidth + 8)
    text: node.value
    enabled: false
}
