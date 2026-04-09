import QtQuick

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World!")
    Rectangle {
        color: "#272822"
        width: root.width
        height: root.height
        Text {
            text: "Hello World!"
            color: "white"
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 20
            }
            font {
                pixelSize: 50
                bold: true
                weight: 10
            }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10
            Rectangle {
                width: 40
                height: 20
                radius: 3.0
                color: '#787878'
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                Text {
                    text: "-"
                    color: "white"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    font {
                        pixelSize: 10
                        bold: true
                        weight: 10
                    }
                }
            }
            Rectangle {
                width: 80
                height: 25
                radius: 20.0
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                color: '#eaeaea'
                border {
                    width: 3
                    color: '#4e4e4e'
                }
                Text {
                    text: "0.0"
                    color: "black"
                    anchors {
                        left: parent.left
                        leftMargin: 10
                        verticalCenter: parent.verticalCenter
                    }
                    font {
                        pixelSize: 10
                        bold: true
                        weight: 10
                    }
                }
            }
            Rectangle {
                width: 40
                height: 20
                radius: 3.0
                color: '#787878'
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                Text {
                    text: "+"
                    color: "white"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        verticalCenter: parent.verticalCenter
                    }
                    font {
                        pixelSize: 10
                        bold: true
                        weight: 10
                    }
                }
            }
        }
    }
}
