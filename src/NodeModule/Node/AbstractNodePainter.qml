import QtQuick
import QtQuick.Shapes
import NodeModule

Shape {
    required property NodeGraphicalObject nodeObject
    required property NavigableArea area
    required property DraftConnection draftConnection

    preferredRendererType: Shape.CurveRenderer
}
