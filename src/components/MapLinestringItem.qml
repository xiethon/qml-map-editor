//! MapPointItem.qml

import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Effects

MapItemGroup {
    id: _control

	property var map

    property var pointColor : "blue"  //! 顶点颜色
    property var lineColor : "blue"     //! 边颜色

    property bool selected: false     //! 是否选中
    property bool closed: false       //! 是否闭合
    property var mapPoints: []         //! 顶点列表

    property var _path: {
        var path = mapPoints.map(function(point) { return point.coordinate; });
        return path;
    } //! 多边形路径

	property var _mouseCoordinate: null     //! 鼠标坐标（用于显示虚线）
    property var _visualPath: !selected ? _path :  closed ? _path : _path.concat(_mouseCoordinate ? [_mouseCoordinate] : []) 

	signal pointClicked(var uuid) //! 设置选中点信号
    signal linSstringClicked()       //! 设置选中多边形信号

	//!Polyline
	MapPolyline {
		id: edgeItem
		path: _control._path
		line{
			width:  4
			color: _control.lineColor
		}	
		TapHandler {
            gesturePolicy: TapHandler.ReleaseWithinBounds
            enabled: !_control.selected
            onTapped: linSstringClicked();
        }
	}

	//! visual line
	MapSegmentItem{
        id: dashItem
        map:                 _control.map
        visible:            mouseArea.enabled
        start:              _control._path.length > 0 ? _control._path[_control._path.length - 1] : null
        end:                _control._mouseCoordinate
        color:              _control.lineColor
        solid:              false
        selected:           true
    }

	//! 多边形顶点
    MapItemView{
        id: vertexListView
        model:     mapPoints
        delegate: MapPointItem {
            selected:           modelData.selected
            coordinate:          modelData.coordinate
            onCoordinateChanged: {
                modelData.coordinate = coordinate
            }
            color:              _control.pointColor
            TapHandler {
                gesturePolicy: TapHandler.ReleaseWithinBounds
                onTapped: pointClicked(modelData.uuid);
			}
            DragHandler {
                id: handler
                target: parent
                enabled:modelData.selected
            }
        }
    }

	//! 获取鼠标坐标
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        enabled: !_control.closed && _control._path.length > 0  && _control.selected
        acceptedButtons: Qt.LeftButton
        propagateComposedEvents: true
        onPositionChanged: (mouse) => {
            _control._mouseCoordinate = _control.map.toCoordinate(Qt.point(mouse.x, mouse.y));
        }
    }
}
