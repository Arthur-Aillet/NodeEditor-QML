pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CutieUiModule
import CutieDesigner.Nodes.Input

FlexboxLayout {
    id: gradientInputControl

    required property GradientInputNode node

    onParentChanged: {
        parent.anchors.verticalCenter = undefined;
        parent.anchors.top = parent.parent.top;
    }

    y: 10

    direction: FlexboxLayout.FlexboxDirection.Column
    alignItems: FlexboxLayout.FlexboxAlignment.AlignCenter
    gap: 4

    FlexboxLayout {
        Button {
            text: "+"
            implicitHeight: 20
            implicitWidth: 30
            onClicked: gradientInputControl.node.colorList.pushColor()
        }

        Button {
            text: "-"
            implicitHeight: 20
            implicitWidth: 30
            onClicked: gradientInputControl.node.colorList.popColor()
        }
        y: 10
    }
    ListView {
        boundsBehavior: Flickable.StopAtBounds
        Layout.preferredHeight: height
        Layout.preferredWidth: width

        width: contentItem.childrenRect.width
        height: contentItem.childrenRect.height > 1 ? contentItem.childrenRect.height + 15 : 1

        model: gradientInputControl.node.colorList

        delegate: FlexboxLayout {
            id: item

            required property int index
            required property color color
            required property real pos

            gap: 10

            ColorPicker {
                Component.onCompleted: {
                    value = item.color;
                }

                onValueChanged: {
                    gradientInputControl.node.colorList.editColor(value, item.index);
                }
            }
            TextField {
                Component.onCompleted: {
                    value = item.pos;
                }

                onValueChanged: {
                    gradientInputControl.node.colorList.editPos(value, item.index);
                }

                textField.inputMethodHints: Qt.ImhFormattedNumbersOnly
                textField.Layout.preferredWidth: Math.max(80, textField.contentWidth + 8)
                textField.validator: DoubleValidator {
                    bottom: 0
                    top: 1
                }
            }
        }
    }
}
