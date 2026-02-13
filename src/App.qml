import QtQuick
import QtLocation
import QtPositioning

import map.editor
import map.editor.components

Window {
    id: app
    visible: true
    width: 1200
    height: 800
    title: qsTr("")

    property alias editorController: editorController

    MapEditorController {
        id: editorController
    }

    MapEditorLayer {
        anchors.fill: parent
    }
}