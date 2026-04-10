import QtQuick

Rectangle {
    id: textInput
    required property real value

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
        text: textInput.value
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
