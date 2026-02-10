import QtQuick
import QtLocation
import QtPositioning

import map.editor
import map.editor.components    

EditorMap {
    id: editorMap
    
    MapEditorToolBar {
        id: editorMapTool
        z: 1
        anchors {
            top: parent.top
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
    }

    MapEditorActionBar {
        id: editorMapAction
        z: 1
        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }
    }

    //! 标记点列表
    MapItemView {
        id: mapPointListView
        z: 1
        model: app.editorController.mapPoints
        delegate: MapPointItem {
            coordinate: modelData.coordinate
            source: "qrc:/qt/qml/map/editor/assets/point.svg"
            color: modelData.selected ? "red" : "white"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true  
                enabled: !app.editorController.locked
                acceptedButtons: Qt.LeftButton
                onClicked: (mouse) => {
                    if (mouse.modifiers & Qt.ControlModifier) {
                        app.editorController.setSelectedItem(modelData.id)
                    } else {
                        app.editorController.setSelectedItemAndClearOthers(modelData.id)
                    }
                }
            } 
        }
    }

    //! 多边形列表
    MapItemView {
        id: mapPolygonListView
        z: 1
        model: app.editorController.mapPolygons
        delegate: MapPolygonItem {
            path: modelData.path
            selected: modelData.selected
            color: "white"
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: !app.editorController.locked
        acceptedButtons: Qt.LeftButton
        onDoubleClicked: (mouse) => {
            app.editorController.append(editorMap.toCoordinate(Qt.point(mouse.x, mouse.y), false))
        }
        onClicked: (mouse) => {
            app.editorController.clearAllSelected()
        }
    }
}