pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts

import NodeEditor

AbstractNodePainter {
    id: defaultNodePainter

    property alias strokeColor: rect.strokeColor
    property alias strokeWidth: rect.strokeWidth

    readonly property int spacing: 10
    readonly property int portSize: 20

    Connections {
        target: defaultNodePainter.nodeObject
        function onConnectionChanged(portIndex: int, portSide: int, otherNodeId: int, otherPortId: int) {
            let port = inOutRepeater.portAt(portIndex, portSide);
            if (port) {
                port.portConnectionChanged();
            }
        }
    }

    override property Item embeddedComponentContainer: embedContainer
    override property var getPortPosition: (portIndex, portSide) => {
        const step = spacing + portSize;

        let totalHeight = content.y;
        if (content.y != 0) {
            totalHeight -= spacing / 2;
        }
        totalHeight += step * (portIndex + 1);
        totalHeight -= spacing / 2;

        if (portSide == NodeEditor.PortSide.In) {
            return Qt.point(0, totalHeight);
        }
        return Qt.point(content.width, totalHeight);
    }

    width: column.width
    height: column.height

    ShapePath {
        id: rect
        property alias nodeObject: defaultNodePainter.nodeObject

        strokeWidth: nodeObject.containsMouse ? nodeObject.style.hoveredPenWidth : nodeObject.style.penWidth
        strokeColor: {
            if (nodeObject.locked)
                return Qt.darker(nodeObject.style.normalBoundaryColor, 1.4);
            // if (invalid) {
            //     return errorColor;
            // } else if (warning) {
            //     return warningColor;
            // } else
            if (nodeObject.selected) {
                return nodeObject.style.selectedBoundaryColor;
            } else {
                return nodeObject.style.normalBoundaryColor;
            }
        }
        fillGradient: LinearGradient {
            x1: 0.0
            y1: 0.0
            x2: 2.0
            y2: defaultNodePainter.height
            stops: [
                GradientStop {
                    position: 0.0
                    color: defaultNodePainter.nodeObject.style.gradientColor0
                },
                GradientStop {
                    position: 0.10
                    color: defaultNodePainter.nodeObject.style.gradientColor1
                },
                GradientStop {
                    position: 0.9
                    color: defaultNodePainter.nodeObject.style.gradientColor2
                },
                GradientStop {
                    position: 1.0
                    color: defaultNodePainter.nodeObject.style.gradientColor3
                }
            ]
        }
        PathRectangle {
            width: defaultNodePainter.width
            height: defaultNodePainter.height
            radius: 3.0
        }
    }

    property string label: ModelInterface.graph.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.Label)
    property bool labelEditable: ModelInterface.graph.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.LabelEditable)
    property string caption: ModelInterface.graph.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.Caption)

    FlexboxLayout {
        id: column
        direction: FlexboxLayout.Column
        alignItems: FlexboxLayout.AlignCenter

        Text {
            id: captionText
            text: defaultNodePainter.caption
            color: {
                let col = defaultNodePainter.nodeObject.style.fontColor;
                if (defaultNodePainter.nodeObject.locked)
                    col = Qt.darker(col, 1.4);
                return col;
            }
            font.bold: true
            // font.bold: defaultNodePainter.label == ""
            // font.italic: defaultNodePainter.label != ""
            property bool active: ModelInterface.graph.nodeData(defaultNodePainter.nodeObject.nodeId, NodeEditor.NodeRole.CaptionVisible)

            leftPadding: 2
            rightPadding: 2
            topPadding: defaultNodePainter.spacing
            bottomPadding: labelText.active ? 0 : defaultNodePainter.spacing / 4
            visible: active
            Layout.maximumHeight: active ? height : 0
        }

        Text {
            id: labelText
            text: defaultNodePainter.label
            color: {
                let col = defaultNodePainter.nodeObject.style.fontColor;
                if (defaultNodePainter.nodeObject.locked)
                    col = Qt.darker(col, 1.4);
                return col;
            }
            property bool active: ModelInterface.graph.nodeData(defaultNodePainter.nodeObject.nodeId, NodeEditor.NodeRole.LabelVisible)

            visible: active
            padding: 1
            leftPadding: 2
            rightPadding: 2
            font.pixelSize: 10
            Layout.maximumHeight: active ? height : 0
        }

        Item {
            id: content
            property real maxLabelWidthIn: inOutRepeater.getMaxLabelWidth(NodeEditor.PortSide.In)

            Layout.fillWidth: true
            Layout.minimumWidth: {
                const maxLabelWidthOut = inOutRepeater.getMaxLabelWidth(NodeEditor.PortSide.Out);

                return maxLabelWidthIn + defaultNodePainter.spacing + embedContainer.width + defaultNodePainter.spacing + maxLabelWidthOut;
            }
            Layout.minimumHeight: {
                const offsetForLabelNodes = content.y === 0 ? 0 : -defaultNodePainter.spacing / 2;
                const maxPortCount = Math.max(defaultNodePainter.nodeObject.inPortCount, defaultNodePainter.nodeObject.outPortCount);
                const portsHeight = maxPortCount * (defaultNodePainter.portSize + defaultNodePainter.spacing);

                return Math.max(embedContainer.height, portsHeight + offsetForLabelNodes) + defaultNodePainter.spacing;
            }

            Repeater {
                id: inOutRepeater
                model: defaultNodePainter.nodeObject.inPortCount + defaultNodePainter.nodeObject.outPortCount
                delegate: DefaultPortPainter {
                    y: -content.y
                    nodePainter: defaultNodePainter as DefaultNodePainter
                }

                function portAt(portId: int, portSide: int): var {
                    let i = 0;
                    let target = defaultNodePainter.nodeObject.inPortCount;
                    if (portSide == NodeEditor.PortSide.Out) {
                        i += defaultNodePainter.nodeObject.inPortCount;
                        target += defaultNodePainter.nodeObject.outPortCount;
                    }
                    for (i; i != target; i++) {
                        const port = itemAt(i) as DefaultPortPainter;
                        if (port.portId == portId)
                            return port;
                    }
                    return undefined;
                }

                function getMaxLabelWidth(side): real {
                    let maxLabelWidth = 0;
                    for (let i = 0; i < count; i++) {
                        const port = itemAt(i) as DefaultPortPainter;
                        if (port.side == side && port.width > maxLabelWidth)
                            maxLabelWidth = port.width;
                    }
                    return maxLabelWidth;
                }
            }

            // Embedded component
            Item {
                id: embedContainer
                x: content.maxLabelWidthIn + defaultNodePainter.spacing
                anchors.verticalCenter: parent.verticalCenter
                height: childrenRect.height
                width: childrenRect.width
                Layout.minimumHeight: childrenRect.height
                Layout.minimumWidth: childrenRect.width
            }
        }
    }
}
