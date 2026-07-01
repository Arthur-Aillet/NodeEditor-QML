pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NodeEditor
import CutieDesigner.Nodes.Process
import CutieUiModule as Cute

FlexboxLayout {
    id: textTyper
    direction: FlexboxLayout.Column

    required property TextTyperNode node

    Item {
        Layout.fillWidth: true
        Layout.minimumWidth: 80
        Layout.minimumHeight: displayArea.height

        TextArea {
            id: displayArea
            width: parent.width - 20
            placeholderText: "Text"
            text: textTyper.node.text
            onTextEdited: textTyper.node.text = text
            Connections {
                target: textTyper.node
                function onTextChanged() {
                    displayArea.text = textTyper.node.text;
                }
            }
        }
        Cute.PlayPauseButton {
            id: playBtn
            anchors.left: displayArea.right
            height: parent.height
            playing: textTyper.node.play
            onClicked: textTyper.node.play = !textTyper.node.play
            Connections {
                target: textTyper.node
                function onPlayChanged() {
                    playBtn.playing = textTyper.node.play;
                }
            }
        }
    }

    ListView {
        id: eventList

        boundsBehavior: Flickable.StopAtBounds
        Layout.preferredHeight: height
        Layout.preferredWidth: width

        height: Math.max(Math.min(150, contentItem.childrenRect.height), 1)
        clip: true
        property double maxWidth: {
            let largest = 0;
            for (let i = 0; i != count; i++) {
                if (itemAtIndex(i).implicitWidth > largest) {
                    largest = itemAtIndex(i).implicitWidth;
                }
            }
            return largest;
        }
        width: maxWidth
        model: textTyper.node.model

        ScrollBar.vertical: ScrollBar {
            anchors.right: parent.right
            visible: (eventList.contentHeight > eventList.height) ? true : false
        }

        MouseArea {
            id: area
            anchors.fill: parent
            acceptedButtons: Qt.NoButton

            onWheel: event => {
                eventList.flick(0, event.angleDelta.y * 6);
            }
        }

        delegate: Rectangle {
            id: line
            required property string name
            required property var value
            required property int index
            width: layout.width + nameText.width + close.width + 10
            implicitWidth: layout.implicitWidth + nameText.width + close.width + 10
            height: layout.height

            function toUInt(text: string): int {
                let val = parseInt(text);
                if (isNaN(val) || !isFinite(val) || val < 0)
                    val = 0;
                return val;
            }

            border.color: StyleCollection.node.normalBoundaryColor
            border.width: StyleCollection.node.penWidth
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: Qt.darker(StyleCollection.node.gradientColor0, 1.1)
                }
                GradientStop {
                    position: 0.1
                    color: Qt.darker(StyleCollection.node.gradientColor1, 1.1)
                }
                GradientStop {
                    position: 0.9
                    color: Qt.darker(StyleCollection.node.gradientColor2, 1.1)
                }
                GradientStop {
                    position: 1.0
                    color: Qt.darker(StyleCollection.node.gradientColor3, 1.1)
                }
            }

            HoverHandler {
                id: hoverHandler
            }

            Text {
                id: nameText
                leftPadding: 5
                anchors.verticalCenter: parent.verticalCenter
                width: 62
                color: Qt.lighter(hoverHandler.hovered ? StyleCollection.node.fontColor : StyleCollection.node.fontColorFaded, 1.8)
                text: line.name
            }

            FlexboxLayout {
                id: layout
                anchors.left: nameText.right

                direction: FlexboxLayout.Row
                justifyContent: FlexboxLayout.JustifyStart
                alignItems: FlexboxLayout.AlignCenter
                gap: 3
                width: Math.max(implicitWidth, eventList.maxWidth - (nameText.width + close.width + 10))

                TextField {
                    id: posBox
                    placeholderText: "pos"
                    Layout.preferredWidth: Math.max(36, contentWidth + 8)
                    visible: line.name !== "Wait"
                    text: visible ? line.value.pos : ""
                    onTextEdited: {
                        textTyper.node.model.editValue(line.index, "pos", line.toUInt(text));
                    }

                    validator: IntValidator {
                        bottom: 0
                    }
                }
                TextField {
                    id: delayBox
                    placeholderText: "delay(s)"
                    Layout.preferredWidth: Math.max(70, contentWidth + 8)
                    visible: line.name === "Wait"
                    text: visible ? line.value.delay : ""
                    onTextEdited: {
                        textTyper.node.model.editValue(line.index, "delay", Number(text));
                    }

                    validator: DoubleValidator {
                        bottom: 0.0
                    }
                }
                TextField {
                    id: amountBox
                    placeholderText: "amount"
                    Layout.preferredWidth: Math.max(70, contentWidth + 8)
                    visible: line.name === "Erase"
                    text: visible ? line.value.amount : ""
                    onTextEdited: {
                        textTyper.node.model.editValue(line.index, "amount", line.toUInt(text));
                    }

                    validator: IntValidator {
                        bottom: 0.0
                    }
                }
                TextArea {
                    id: textBox
                    Layout.preferredWidth: Math.min(Math.max(50, contentWidth + 16), 300)
                    visible: line.name === "Insert" || line.name === "Replace"
                    text: visible ? line.value.text : ""
                    onTextEdited: {
                        textTyper.node.model.editValue(line.index, "text", text);
                    }

                    placeholderText: "text"
                }
            }
            RoundButton {
                id: close
                anchors.left: layout.right
                anchors.leftMargin: 5
                enabled: true
                anchors.verticalCenter: parent.verticalCenter
                width: 16
                height: 16
                rightPadding: 6
                onClicked: textTyper.node.model.removeEvent(line.index)
                text: "\u2717" // Unicode Character 'BALLOT X'
                radius: 8
            }
        }
    }
    ComboBox {
        Layout.fillWidth: true
        displayText: "Add event"
        model: ["Wait", "Erase", "Insert", "Replace"]
        currentIndex: -1
        onActivated: index => {
            textTyper.node.model.addEvent(model[index]);
            currentIndex = -1;
        }
    }
}
