pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import CutieDesigner.Nodes.Display

FlexboxLayout {
    id: atype
    required property ATypeNode node

    anchors.fill: parent
    direction: FlexboxLayout.Row
    wrap: FlexboxLayout.Wrap
    justifyContent: FlexboxLayout.JustifyStart
    gap: 15

    Component.onCompleted: {
        applyText(node.text);
    }

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
        if (!node.character)
            return;

        let currentText = node.character.getString(atype);
        let commonStart = sharedStart(text, currentText);
        let remaining = text.substr(commonStart.length, text.length);
        let cleanedCurrentText = currentText.substr(commonStart.length, currentText.length);
        let commonEnd = sharedEnd(remaining, cleanedCurrentText);
        remaining = remaining.substr(0, remaining.length - commonEnd.length);
        cleanedCurrentText = cleanedCurrentText.substr(0, cleanedCurrentText.length - commonEnd.length);

        for (let i = 0; i != remaining.length; i++) {
            if (i < cleanedCurrentText.length) {
                node.character.setChar(atype, commonStart.length + i, remaining[i]);
            } else {
                node.character.createCharacterObject(atype, remaining[i], commonStart.length + i);
            }
        }
        for (let i = 0; i < cleanedCurrentText.length - remaining.length; i++) {
            node.character.destroyItem(atype, commonStart.length + remaining.length + i);
        }
    }

    Connections {
        target: atype.node
        function onTextChanged() {
            atype.applyText(atype.node.text);
        }

        function onCharChanged() {
            if (!atype.node.character) {
                atype.children = "";
            } else {
                atype.applyText(atype.node.text);
            }
        }
    }
}
