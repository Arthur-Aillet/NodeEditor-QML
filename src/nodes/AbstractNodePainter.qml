import QtQuick
import QtQuick.Shapes
import NodeEditor

Shape {
    required property NodeObject nodeObject
    required property NavigableArea area
    required property DraftConnection draftConnection
    required property AbstractContext context

    virtual property Item embeddedComponentContainer: null

    virtual property var getPortPosition: (portIndex, portSide) => {
        console.error("\"getPortPosition\": Implementation missing");
    }

    preferredRendererType: Shape.CurveRenderer
}
