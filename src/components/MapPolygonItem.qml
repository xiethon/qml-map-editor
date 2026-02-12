import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
 
MapItemGroup{
    id: _control
 
    property var map
    property var color : "blue"         //! 多边形颜色
    property var pointColor : "blue"  //! 顶点颜色
    property var lineColor : "blue"     //! 边颜色

    property bool selected: false     //! 是否选中
    property bool closed: false       //! 是否闭合
    property var mapPoints: []         //! 多边形顶点列表

    property var _path: {
        var path = mapPoints.map(function(point) { return point.coordinate; });
        if (closed && path.length > 0) {
            path = path.concat([path[0]]);
        }
        return path;
    } //! 多边形路径

    property var _mouseCoordinate: null     //! 鼠标坐标（用于显示虚线）
    property var _visualPath: !selected ? _path :  closed ? _path : _path.concat(_mouseCoordinate ? [_mouseCoordinate] : []) 

    signal pointClicked(var uuid) //! 设置选中点信号
    signal polygonClicked()       //! 设置选中多边形信号

    //! 多边形
    MapPolygon {
        id:             polygonItem
        path:           _control._visualPath
        color:          _control.color
        opacity:        _control.selected ? 0.6 : 0.1
        border{
            width: 1
            color: polygonItem.color
        }
        TapHandler {
            gesturePolicy: TapHandler.ReleaseWithinBounds
            enabled: !_control.selected
            onTapped: polygonClicked();
        }
    }

    //! 多边形边
    MapPolyline {
        id: edgeItem
        path: _control._path
        line{
            width: 2
            color: _control.lineColor
        }  
    }

    //! 多边形close虚线（虚线显示）
    MapSegmentItem{
        id: dashItem
        map:                 _control.map
        visible:            mouseArea.enabled
        start:              _control._path.length > 0 ? _control._path[_control._path.length - 1] : null
        end:                _control._mouseCoordinate
        color:              lineColor
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