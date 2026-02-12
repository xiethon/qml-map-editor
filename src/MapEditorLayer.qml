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
        enabled: !app.editorController.locked
        id: mapMarkerListView
        z: 1
        model: app.editorController.mapMarkers
        delegate: MapMarkerItem {
            coordinate: modelData.coordinate
            address: modelData.address
            selected: modelData.selected
            source: "qrc:/qt/qml/map/editor/assets/marker.svg"
            color: modelData.selected ? "red" : "white"
            
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true  
                enabled: !app.editorController.locked
                acceptedButtons: Qt.LeftButton
                onClicked: (mouse) => {
                    if (mouse.modifiers & Qt.ControlModifier) {
                        app.editorController.setSelectedItem(modelData.uuid)
                    } else {
                        app.editorController.setSelectedItemAndClearOthers(modelData.uuid)
                    }
                }
            }
            Component.onCompleted: {
                //! 逆地理编码获取地址信息
                if (modelData.address === "" && modelData.selected) {
                    editorMap.reverseGeocode(modelData.coordinate, (address, error) => {
                        modelData.address = address
                    })
                }
            }
            onCoordinateChanged: {
                modelData.coordinate = coordinate
            }
            onDragFinished: {
                editorMap.reverseGeocode(modelData.coordinate, (address, error) => {
                        modelData.address = address
                    })
            }
        }
    }

    //! 多边形列表
    MapItemView {
        id: mapPolygonListView
        enabled: !app.editorController.locked && app.editorController.editMode === MapEditorController.POLYGON
        z: 1
        model: app.editorController.mapPolygons
        delegate: MapPolygonItem {
            id: _mapPolygonItem
            map: editorMap
            color: "white"
            edgeColor: "blue"

            closed: modelData.closed
            selected: modelData.selected
            mapPoints: modelData.mapPoints

            onPolygonClicked: {
                app.editorController.setSelectedItemAndClearOthers(modelData.uuid)
            }
            onPointClicked: (uuid) => {
                app.editorController.setSelectedItemAndClearOthers(modelData.uuid)
                modelData.setSelectedPoint(uuid)
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: !app.editorController.locked
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        //! 双击左键地图时，添加一个点
        onDoubleClicked: (mouse) => {
            if (mouse.button === Qt.LeftButton) {
                app.editorController.append(editorMap.toCoordinate(Qt.point(mouse.x, mouse.y)))
            }
        }

        //! 结束当前当前编辑对象的操作
        onClicked: (mouse) => {
            if (mouse.button === Qt.RightButton) {
                app.editorController.finishCurrentEditing()
            }
        }
    }
}