import QtQuick
import NodeEditor

Canvas {
    required property ConnectionObject obj
    required property connectionStyle style

    virtual property rect boundingBox

    virtual property var distanceToCurve: point => {
        console.error("\"distanceToCurve\": Implementation missing");
    }
}
