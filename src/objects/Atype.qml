pragma ComponentBehavior: Bound
import QtQuick
import CutieDesignerModule
import QtQuick.Layouts

FlexboxLayout {
    id: root
    property string text

    anchors.fill: parent
    direction: FlexboxLayout.Row
    wrap: FlexboxLayout.Wrap
    justifyContent: FlexboxLayout.JustifyStart
    gap: 15

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

    onTextChanged: {
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
