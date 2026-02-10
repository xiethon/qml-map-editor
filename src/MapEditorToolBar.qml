import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import map.editor.components

RowLayout{
    id: mapEditorToolBar
    spacing: 2

    Button {
        text: qsTr("Point")
        checkable: true
        enabled: !app.editorController.locked
        checked: app.editorController.editMode === MapEditorController.POINT
        onClicked: app.editorController.editMode = MapEditorController.POINT
    }
    Button {
        text: qsTr("Linestring")
        checkable: true
        enabled: !app.editorController.locked
        checked: app.editorController.editMode === MapEditorController.LINESTRING
        onClicked: app.editorController.editMode = MapEditorController.LINESTRING
    }
    Button {
        text: qsTr("Polygon")
        checkable: true
        enabled: !app.editorController.locked
        checked: app.editorController.editMode === MapEditorController.POLYGON
        onClicked: app.editorController.editMode = MapEditorController.POLYGON
    }
    Button {
        text: qsTr("Circle")
        checkable: true
        enabled: !app.editorController.locked
        checked: app.editorController.editMode === MapEditorController.CIRCLE
        onClicked: app.editorController.editMode = MapEditorController.CIRCLE
    }
    Button {
        text: qsTr("Square")
        checkable: true
        enabled: !app.editorController.locked
        checked: app.editorController.editMode === MapEditorController.SQUARE
        onClicked: app.editorController.editMode = MapEditorController.SQUARE
    }
}
