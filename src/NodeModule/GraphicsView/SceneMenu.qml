pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import NodeModule

Menu {
    id: root
    popupType: Popup.Window
    focus: true

    property var categories: DataFlowModelInterface.registery.categoryModel

    TextField {
        id: searchField
        placeholderText: qsTr("Filter")
        Component.onCompleted: () => {
            searchField.forceActiveFocus(Qt.PopupFocusReason);
        }
        onTextChanged: filterModel.invalidate()
    }

    SortFilterProxyModel {
        id: filterModel
        model: root.categories
        filters: [
            FunctionFilter {
                function filter(data: Category): bool {
                    return data.edit.toLowerCase().includes(searchField.text.toLowerCase());
                }
            }
        ]
    }

    component Category: QtObject {
        property string edit
    }

    ListView {
        height: 200
        model: filterModel

        delegate: MenuItem {
            required property string edit
            text: root.underlineSearch(edit)
        }
    }

    function underlineSearch(text: string): string {
        const re = new RegExp(searchField.text, 'gi'); // global, insensitive
        return text.replace(re, `<u>$&</u>`);
    }
}
