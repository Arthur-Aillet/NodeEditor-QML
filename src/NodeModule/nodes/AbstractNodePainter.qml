import QtQuick
import QtQuick.Shapes
import NodeModule

Shape {
    required property NodeGraphicalObject nodeObject
    required property NavigableArea area
    required property DraftConnection draftConnection

    virtual property var getPortPosition: portIndex => {
        console.error("\"getPortPosition\": Implementation missing");
    }

    preferredRendererType: Shape.CurveRenderer
}
