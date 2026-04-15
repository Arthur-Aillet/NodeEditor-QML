import QtQuick
import Sandbox

Rectangle {
    id: colorScene
    ColorSceneReceiver {
        onValueChanged: () => {
            colorScene.height = 100 + (value % 40);
        }
    }
    width: 100
    height: 100
    color: "blue"
}
