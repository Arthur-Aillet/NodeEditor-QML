import QtQuick
import "InnerShadow"

Rectangle {
    id: paneBackground
    color: palette.base.hslLightness > 0.5 ? palette.midlight : Qt.darker(palette.dark, 1.4)

    Item {
        id: background
        anchors.fill: parent

        layer.enabled: true
        layer.smooth: true
        visible: false

        Rectangle {
            id: inner
            width: parent.width - 4
            height: parent.height - 4
            x: 2
            y: 2
            color: palette.base.hslLightness > 0.5 ? Qt.darker(palette.mid, 1.05) : palette.mid
        }
    }

    InnerShadow {
        anchors.fill: background
        source: background
        innerShadowColor: paneBackground.color
    }
}
