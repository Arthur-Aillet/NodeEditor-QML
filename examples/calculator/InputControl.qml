import QtQuick
import QtQuick.Controls
import examples.calculator

TextField {
    id: inputControl

    required property InputNode node

    validator: DoubleValidator {}
    width: Math.max(40, contentWidth + 8)
    text: node.value
    onTextEdited: {
        node.value = text;
    }
}
