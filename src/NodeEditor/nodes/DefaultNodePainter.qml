pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts

import NodeEditor

AbstractNodePainter {
    id: root

    property alias strokeColor: rect.strokeColor
    property alias strokeWidth: rect.strokeWidth

    readonly property int spacing: 10
    readonly property int portSize: 20

    Connections {
        target: root.nodeObject
        function onConnectionChanged(portIndex: int, portType: int, otherNodeId: int, otherPortId: int) {
            inOutRepeater.portAt(portIndex, portType).portConnectionChanged();
        }
    }

    override property Item embeddedComponentContainer: embedContainer
    override property var getPortPosition: (portIndex, portType) => {
        const step = spacing + portSize;

        let totalHeight = content.y;
        if (content.y != 0) {
            totalHeight -= spacing / 2;
        }
        totalHeight += step * (portIndex + 1);
        totalHeight -= spacing / 2;

        if (portType == NodeEditor.PortType.In) {
            return Qt.point(0, totalHeight);
        }
        return Qt.point(content.width, totalHeight);
    }

    width: column.width
    height: column.height

    ShapePath {
        id: rect
        property alias nodeObject: root.nodeObject

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

    property string label: ModelInterface.graph.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.Label)
    property bool labelEditable: ModelInterface.graph.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.LabelEditable)
    property string caption: ModelInterface.graph.nodeData(nodeObject.nodeId, NodeEditor.NodeRole.Caption)

    FlexboxLayout {
        id: column
        direction: FlexboxLayout.Column
        alignItems: FlexboxLayout.AlignCenter

        Text {
            id: captionText
            text: root.caption
            color: {
                let col = root.nodeObject.style.fontColor;
                if (root.nodeObject.locked)
                    col = Qt.darker(col, 1.4);
                return col;
            }
            font.bold: true
            // font.bold: root.label == ""
            // font.italic: root.label != ""
            property bool active: ModelInterface.graph.nodeData(root.nodeObject.nodeId, NodeEditor.NodeRole.CaptionVisible)

            leftPadding: 2
            rightPadding: 2
            topPadding: root.spacing
            bottomPadding: labelText.active ? 0 : root.spacing / 4
            visible: active
            Layout.maximumHeight: active ? height : 0
        }

        Text {
            id: labelText
            text: root.label
            color: {
                let col = root.nodeObject.style.fontColor;
                if (root.nodeObject.locked)
                    col = Qt.darker(col, 1.4);
                return col;
            }
            property bool active: ModelInterface.graph.nodeData(root.nodeObject.nodeId, NodeEditor.NodeRole.LabelVisible)

            visible: active
            padding: 1
            leftPadding: 2
            rightPadding: 2
            font.pixelSize: 10
            Layout.maximumHeight: active ? height : 0
        }

        Item {
            id: content
            property real maxLabelWidthIn: inOutRepeater.getMaxLabelWidth(NodeEditor.PortType.In)

            Layout.fillWidth: true
            Layout.minimumWidth: {
                const maxLabelWidthOut = inOutRepeater.getMaxLabelWidth(NodeEditor.PortType.Out);

                return maxLabelWidthIn + root.spacing + embedContainer.width + root.spacing + maxLabelWidthOut;
            }
            Layout.minimumHeight: {
                const offsetForLabelNodes = content.y === 0 ? 0 : -root.spacing / 2;
                const maxPortCount = Math.max(root.nodeObject.inPortCount, root.nodeObject.outPortCount);
                const portsHeight = maxPortCount * (root.portSize + root.spacing);

                return Math.max(embedContainer.height, portsHeight + offsetForLabelNodes) + root.spacing;
            }

            Repeater {
                id: inOutRepeater
                model: root.nodeObject.inPortCount + root.nodeObject.outPortCount
                delegate: DefaultPortPainter {
                    y: -content.y
                    nodePainter: root as DefaultNodePainter
                }

                function portAt(portId: int, portType: int): var {
                    let i = 0;
                    let target = root.nodeObject.inPortCount;
                    if (portType == NodeEditor.PortType.Out) {
                        i += root.nodeObject.inPortCount;
                        target += root.nodeObject.outPortCount;
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
                x: content.maxLabelWidthIn + root.spacing
                anchors.verticalCenter: parent.verticalCenter
                height: childrenRect.height
                width: childrenRect.width
                Layout.minimumHeight: childrenRect.height
                Layout.minimumWidth: childrenRect.width
            }
        }
    }
}
