pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import CutieDesignerModule

FlexboxLayout {
    id: root
    required property ATypeNode node

    anchors.fill: parent
    direction: FlexboxLayout.Row
    wrap: FlexboxLayout.Wrap
    justifyContent: FlexboxLayout.JustifyStart
    gap: 15

    Component.onCompleted: applyText(node.text)

    function sharedStart(fst, snd) {
        let i = 0;

        while (i != fst.length && i != snd.length && fst.charAt(i) === snd.charAt(i)) {
            i++;
        }
        return fst.substring(0, i);
    }

    function sharedEnd(fst, snd) {
        let i = 0;

        while (i != fst.length && i != snd.length && fst.charAt(fst.length - i - 1) === snd.charAt(snd.length - i - 1)) {
            i++;
        }
        return fst.substring(fst.length - i, fst.length);
    }

    function applyText(text: string) {
        let currentText = "";
        for (let i = 0; i != textModel.count; i++) {
            currentText = currentText.concat(textModel.get(i).character);
        }

        let commonStart = sharedStart(text, currentText);
        let remaining = text.substr(commonStart.length, text.length);
        let cleanedCurrentText = currentText.substr(commonStart.length, currentText.length);
        let commonEnd = sharedEnd(remaining, cleanedCurrentText);
        remaining = remaining.substr(0, remaining.length - commonEnd.length);
        cleanedCurrentText = cleanedCurrentText.substr(0, cleanedCurrentText.length - commonEnd.length);

        for (let i = 0; i != remaining.length; i++) {
            if (i < cleanedCurrentText.length) {
                textModel.set(commonStart.length + i, {
                    character: remaining[i]
                });
            } else {
                textModel.insert(commonStart.length + i, {
                    character: remaining[i]
                });
            }
        }
        for (let i = 0; i < cleanedCurrentText.length - remaining.length; i++) {
            textModel.remove(commonStart.length + remaining.length);
        }
    }

    Connections {
        target: root.node
        function onTextChanged() {
            root.applyText(root.node.text);
        }
    }

    ListModel {
        id: textModel
    }

    Repeater {
        id: rep
        model: textModel
        delegate: UkrugCharacter {
            required property string character

            char: character
        }
    }
}
