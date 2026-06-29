import QtQuick
import QtQuick.Controls

Button {
    property bool playing

    implicitHeight: 30
    implicitWidth: 20
    text: playing ? "⏸" : "⏵"
}
