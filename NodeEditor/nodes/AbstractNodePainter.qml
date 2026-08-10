import QtQuick
import QtQuick.Shapes
import NodeEditor

Shape {
    required property NodeGraphicalObject nodeObject
    required property NavigableArea area
    required property DraftConnection draftConnection

    virtual property Item embeddedComponentContainer: null

    virtual property var getPortPosition: portIndex => {
        console.error("\"getPortPosition\": Implementation missing");
    }

    preferredRendererType: Shape.CurveRenderer
}
