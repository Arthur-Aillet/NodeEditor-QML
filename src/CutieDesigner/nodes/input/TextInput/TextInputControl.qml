import QtQuick
import QtQuick.Controls
import CutieDesigner.Nodes.Input

TextArea {
    required property TextInputNode node

    onTextEdited: {
        node.text = text;
    }
}
