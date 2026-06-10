import QtQuick

QtObject {
    id: root
    property var inner: new Set()

    signal changed

    function add(val) {
        inner.add(val);
        changed();
    }

    function remove(val) {
        inner.delete(val);
        changed();
    }

    function clear() {
        inner.clear();
        changed();
    }

    function has(val) {
        return inner.has(val);
    }
}
