import QtQuick
import QtLocation
import QtPositioning

import map.editor
import map.editor.components    

EditorMap {
    id: editorMap
    
    //! 编辑模式选择工具栏
    MapEditorToolBar {
        id: editorMapTool
        z: 1
        anchors {
            top: parent.top
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
    }

    //! 编辑操作工具栏
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
                acceptedButtons: Qt.LeftButton
                onClicked: (mouse) => {
                    if (mouse.modifiers & Qt.ControlModifier) { //! Ctrl+点击可以多选
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

    //! 线列表
    MapItemView {
        id: mapLinestringListView
        enabled: !app.editorController.locked && app.editorController.editMode === MapEditorController.LINESTRING
        z: 1
        model: app.editorController.mapLineStrings
        delegate: MapLinestringItem {
            id: _mapLinestringItem
            map: editorMap
            lineColor: modelData.selected ? "blue" : "white"
            pointColor: "blue"

            closed: modelData.closed
            selected: modelData.selected
            mapPoints: modelData.mapPoints

            onLinSstringClicked: {
                app.editorController.setSelectedItemAndClearOthers(modelData.uuid)
            }
            onPointClicked: (uuid) => {
                app.editorController.setSelectedItemAndClearOthers(uuid)
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
            lineColor: "blue"
            pointColor: "blue"

            closed: modelData.closed
            selected: modelData.selected
            mapPoints: modelData.mapPoints

            onPolygonClicked: {
                app.editorController.setSelectedItemAndClearOthers(modelData.uuid)
            }
            onPointClicked: (uuid) => {
                app.editorController.setSelectedItemAndClearOthers(uuid)
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
                app.editorController.finishCurrentEditGeometry()
            }
        }
    }
}