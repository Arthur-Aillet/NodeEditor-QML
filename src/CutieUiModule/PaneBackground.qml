import QtQuick
import "InnerShadow"

Rectangle {
    id: paneBackground
    color: palette.base.hslLightness > 0.5 ? palette.base : Qt.darker(palette.base, 2)

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
            color: palette.base.hslLightness > 0.5 ? Qt.darker(palette.base, 1.2) : palette.base
        }
    }

    InnerShadow {
        anchors.fill: background
        source: background
        innerShadowColor: paneBackground.color
    }
}
