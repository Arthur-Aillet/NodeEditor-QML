pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes
import NodeModule

AbstractNodePainter {
    id: root

    property size size: ModelInterface.nodeGeometry.size(nodeObject.nodeId)

    width: size.width
    height: size.height

    property alias strokeColor: rect.strokeColor
    property alias strokeWidth: rect.strokeWidth

    ShapePath {
        id: rect
        property alias nodeObject: root.nodeObject

        strokeWidth: nodeObject.hovered ? nodeObject.style.hoveredPenWidth : nodeObject.style.penWidth
        strokeColor: {
            // if (invalid) {
            //     return errorColor;
            // } else if (warning) {
            //     return warningColor;
            // } else
            if (nodeObject.selected) {
                nodeObject.style.selectedBoundaryColor;
            } else {
                nodeObject.style.normalBoundaryColor;
            }
        }
        fillGradient: LinearGradient {
            x1: 0.0
            y1: 0.0
            x2: 2.0
            y2: root.height
            stops: [
                GradientStop {
                    position: 0.0
                    color: root.nodeObject.style.gradientColor0
                },
                GradientStop {
                    position: 0.10
                    color: root.nodeObject.style.gradientColor1
                },
                GradientStop {
                    position: 0.9
                    color: root.nodeObject.style.gradientColor2
                },
                GradientStop {
                    position: 1.0
                    color: root.nodeObject.style.gradientColor3
                }
            ]
        }
        PathRectangle {
            width: root.width
            height: root.height
            radius: 3.0
        }
    }

    Repeater {
        id: inOutRepeater
        model: root.nodeObject.inPortCount + root.nodeObject.outPortCount
        delegate: DefaultPortPainter {
            nodePainter: root as DefaultNodePainter
        }
    }

    property string label: ModelInterface.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.Label)
    property bool labelEditable: ModelInterface.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.LabelEditable)
    property string caption: ModelInterface.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.Caption)

    property var capPos: ModelInterface.nodeGeometry.captionPosition(nodeObject.nodeId)
    property var capRect: ModelInterface.nodeGeometry.captionRect(nodeObject.nodeId)

    FontMetrics {
        id: fontMetrics
        font.family: captionText.font.family
        font.bold: captionText.font.bold
        font.italic: captionText.font.italic
    }

    Text {
        id: captionText
        text: root.caption
        color: root.nodeObject.style.fontColor
        font.bold: root.label == ""
        font.italic: root.label != ""
        visible: ModelInterface.nodeData(root.nodeObject.nodeId, NodeEditor.NodeRole.CaptionVisible)

        x: parent.capPos.x + parent.capRect.width / 2.0 - fontMetrics.boundingRect(root.caption).width / 2.0
        y: parent.capPos.y - fontMetrics.ascent
    }

    Text {
        text: root.label
        color: root.nodeObject.style.fontColor
        visible: ModelInterface.nodeData(root.nodeObject.nodeId, NodeEditor.NodeRole.LabelVisible)
        anchors.horizontalCenter: parent.horizontalCenter

        x: parent.capPos.x + parent.capRect.width / 2.0
        y: parent.capPos.y - fontMetrics.height - 2.0 - fontMetrics.ascent
    }
}
