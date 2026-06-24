import QtQuick
import QtQuick.Controls

Button {
    id: mediaButton
    enum MediaButtonType {
        Play = 0,
        Pause = 1,
        Stop = 2,
        Record = 3,
        Foward = 4,
        Backward = 5,
        PlayPause = 6
    }

    property int type: 2
    implicitHeight: 30
    implicitWidth: 20
    text: {
        switch (type) {
        case 0:
            return "⏵";
        case 1:
            return "⏸";
        case 2:
            return "⏹";
        case 3:
            return "⏺";
        case 4:
            return "⏭";
        case 5:
            return "⏮";
        case 6:
            return "⏯";
        }
    }
}
