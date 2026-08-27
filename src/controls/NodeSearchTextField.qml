import QtQuick
import QtQuick.Controls

TextField {
    id: searchField
    required property ListView listView

    placeholderText: "Filter"
    onTextChanged: {
        listView.currentIndex = -1;
    }
    hoverEnabled: true
    onHoveredChanged: {
        if (hovered) {
            forceActiveFocus(Qt.PopupFocusReason);
        }
    }

    Keys.onDownPressed: {
        listView.incrementCurrentIndex();
    }
    Keys.onUpPressed: {
        listView.decrementCurrentIndex();
    }
    Keys.onTabPressed: {
        listView.incrementCurrentIndex();
    }
    Keys.onBacktabPressed: {
        listView.decrementCurrentIndex();
    }
}
