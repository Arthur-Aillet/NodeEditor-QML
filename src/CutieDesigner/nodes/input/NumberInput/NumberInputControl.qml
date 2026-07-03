import QtQuick
import QtQuick.Controls
import CutieUiModule

TextField {
    id: textField
    textField.validator: DoubleValidator {}

    //Dirty to refactor
    signal textEdited
    Connections {
        target: textField.textField
        function onTextEdited() {
            textField.textEdited();
        }
    }
}
