import QtQuick
import NodeEditor

SortFilterProxyModel {
    id: sfpm
    required property string filterText
    onFilterTextChanged: {
        invalidate();
    }
    sorters: [
        RoleSorter {
            roleName: "category"
            priority: 0
        },
        RoleSorter {
            roleName: "name"
            priority: 1
        }
    ]
    filters: [
        FunctionFilter {
            function filter(data: RoleData): bool {
                if (data["name"] == "Result")
                    return false;
                if (sfpm.filterText == "")
                    return true;
                return data["name"].toLowerCase().includes(sfpm.filterText.toLowerCase());
            }
        }
    ]

    component RoleData: QtObject {
        property string name
        property string category
        property list<portInfo> portsInfo
    }
}
