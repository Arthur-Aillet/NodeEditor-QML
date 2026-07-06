import QtQuick

MouseArea {
    id: nodeMenuItem
    required property string name
    required property string category
    required property int index
    required property int currentIndex
    required property var replaceRegex

    height: 25
    propagateComposedEvents: true
    hoverEnabled: true

    Rectangle {
        anchors.fill: parent
        color: nodeMenuItem.currentIndex == nodeMenuItem.index ? palette.highlight : ((nodeMenuItem.index % 2 !== 0) ? palette.base : palette.alternateBase)
    }
    Text {
        id: category
        text: " " + nodeMenuItem.category + "  🞂  "
        color: nodeMenuItem.currentIndex == nodeMenuItem.index ? palette.highlightedText : palette.placeholderText
        opacity: nodeMenuItem.currentIndex == nodeMenuItem.index ? 1 : 0.8
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        function computeName(): string {
            return nodeMenuItem.name.replace(nodeMenuItem.replaceRegex, `<u>$&</u>`);
        }

        text: computeName()
        color: nodeMenuItem.currentIndex == nodeMenuItem.index ? palette.highlightedText : palette.text
        opacity: nodeMenuItem.currentIndex == nodeMenuItem.index ? 1 : 0.8
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: category.right
    }
}
