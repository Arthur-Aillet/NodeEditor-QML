pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes

import NodeEditor

AbstractNodePainter {
    id: painter

    override property Item embeddedComponentContainer: painter
    override property var getPortPosition: (portIndex, portSide) => {
        if (portSide == NodeEditor.PortSide.In) {
            return Qt.point(0, 25);
        }
        return Qt.point(width, 25);
    }

    width: 150
    height: 50

    ShapePath {
        strokeWidth: painter.nodeObject.containsMouse ? painter.nodeObject.style.hoveredPenWidth : painter.nodeObject.style.penWidth
        strokeColor: {
            if (painter.nodeObject.locked)
                return Qt.darker(painter.nodeObject.style.normalBoundaryColor, 1.4);
            // if (invalid) {
            //     return errorColor;
            // } else if (warning) {
            //     return warningColor;
            // } else
            if (painter.nodeObject.selected) {
                return painter.nodeObject.style.selectedBoundaryColor;
            } else {
                return painter.nodeObject.style.normalBoundaryColor;
            }
        }
        fillColor: painter.nodeObject.style.gradientColor2
        PathRectangle {
            width: painter.width
            height: painter.height
            radius: Math.min(painter.height, painter.width) / 2
        }
    }

    Text {
        text: painter.context.graphModel.nodeData(painter.nodeObject.nodeId, NodeEditor.NodeRole.Caption)
        color: {
            let col = painter.nodeObject.style.fontColor;
            if (painter.nodeObject.locked)
                col = Qt.darker(col, 1.4);
            return col;
        }

        anchors.centerIn: parent
        font.bold: true
        font.pixelSize: 30
    }

    Repeater {
        model: painter.nodeObject.inPortCount + painter.nodeObject.outPortCount
        delegate: DefaultPortPainter {
            nodePainter: painter
        }
    }
}
