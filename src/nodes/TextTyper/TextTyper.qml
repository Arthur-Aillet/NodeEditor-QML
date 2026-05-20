pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NodeModule

FlexboxLayout {
    direction: FlexboxLayout.Column

    required property var model

    ListView {
        id: eventList
        width: 200
        boundsBehavior: Flickable.StopAtBounds
        property real textHeight: 30
        height: Math.min(6, model.rowCount()) * textHeight + (footerItem !== null ? footerItem.implicitHeight : 0)
        clip: true
        model: parent.model

        onModelChanged: {
            console.log("changed?" + model.rowCount());
        }

        ScrollBar.vertical: ScrollBar {
            anchors.right: parent.right
            visible: (eventList.contentHeight > eventList.height && area.containsMouse == true) ? true : false
        }

        MouseArea {
            id: area
            anchors.fill: parent
            preventStealing: true

            onWheel: event => {
                eventList.flick(0, event.angleDelta.y * 5);
            }
        }

        delegate: Rectangle {
            id: line
            required property string name

            width: 200
            height: eventList.textHeight
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

            Component.onCompleted: {
                console.log("here " + name);
            }
            Text {
                id: text
                color: Qt.lighter(hoverHandler.hovered ? StyleCollection.node.fontColor : StyleCollection.node.fontColorFaded, 1.8)
                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 3
                text: line.name
            }
        }
    }
    ComboBox {
        Layout.fillWidth: true
        displayText: "Add event"
        model: ["Wait", "Remove", "Insert", "Replace"]
        onActivated: index => {
        // eventList.model += ",";
        // eventList.model += model[index];
        }
    }
}
