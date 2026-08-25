import QtQuick
import NodeEditor

Canvas {
    required property ConnectionObject obj

    virtual property rect boundingBox

    virtual property var distanceToCurve: point => {
        console.error("\"distanceToCurve\": Implementation missing");
    }
}
