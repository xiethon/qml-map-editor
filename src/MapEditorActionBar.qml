import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import map.editor.components

ColumnLayout{
    id: mapEditorActionBar
    spacing: 1

    //! 锁定按钮
    MapEditorActionButton {
        source: app.editorController.locked ? "qrc:/qt/qml/map/editor/assets/lock.svg" : "qrc:/qt/qml/map/editor/assets/unlock.svg"
        toolTipText: app.editorController.locked ? qsTr("Unlock Editing") : qsTr("Lock Editing")
        onClicked: app.editorController.locked = !app.editorController.locked
    }

    //! 清除按钮
    MapEditorActionButton {
        source: "qrc:/qt/qml/map/editor/assets/clean.svg"
        toolTipText: qsTr("Clear All")
        enabled: !app.editorController.locked
        onClicked: app.editorController.clearAll()
    }

    //! 删除按钮
    MapEditorActionButton {
        source: "qrc:/qt/qml/map/editor/assets/delete.svg"
        toolTipText: qsTr("Delete Selected")
        enabled: !app.editorController.locked
        onClicked: app.editorController.deleteSelected()
    }
}
