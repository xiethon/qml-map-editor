import QtQuick
import QtLocation
import QtPositioning

import map.editor
import map.editor.components

Window {
    id: app
    visible: true
    width: 800
    height: 600
    title: qsTr("")

    property alias editorController: editorController

    MapEditorController {
        id: editorController
    }

    MapEditorLayer {
        anchors.fill: parent
    }
}