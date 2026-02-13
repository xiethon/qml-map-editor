import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Effects

MapItemGroup {
    id: _control

	property var map

    property var color : "blue"  //! 顶点颜色
    property var borderColor : "blue"     //! 边颜色

    property bool selected: false     //! 是否选中
    property bool firstEditFinished: false
    property bool closed: false       //! 是否闭合
    property var center: null         //! 圆心坐标
    property var radius: 0            //! 圆半径
     
    property var _mouseCoordinate: null //! 鼠标坐标

    signal circleClicked()       //! 设置选中圆信号

    MapCircle {
        id: circleItem
        center: _control.center
        radius: _control.radius
        visible: _control.center !== null && _control.radius > 0
        color: _control.color
        border.width: 2
        border.color: _control.borderColor
        opacity:  0.4
        TapHandler {
            gesturePolicy: TapHandler.ReleaseWithinBounds
            enabled: !_control.selected
            onTapped: circleClicked();
        }
    }

    //! visual line
	MapSegmentItem{
        id: dashItem
        map:                 _control.map
        visible:            _control.selected
        start:              _control.center
        end:                _control.closed ? edgeItem.coordinate : _control._mouseCoordinate
        color:              _control.borderColor
        solid:              false
        selected:           true
    }

    //! 圆心
    MapQuickItem {
        id: centerItem
        coordinate: _control.center
        visible: _control.center !== null && _control.selected
        anchorPoint.x: centerRect.width / 2
        anchorPoint.y: centerRect.height / 2
        sourceItem: Rectangle {
            id: centerRect
            width: 6
            height: 6
            color: "white"
            radius: 3
        }
    }

    //! 边缘控制点
    MapQuickItem {
        id: edgeItem
        visible: _control.selected && _control.closed
        anchorPoint.x: edgeRect.width / 2
        anchorPoint.y: edgeRect.height / 2
        sourceItem: Rectangle {
            id: edgeRect
            width: 8
            height: 8
            color: _control.color
            border.width: 1
            border.color: _control.borderColor
            radius: 4
        }
        
        Binding on coordinate {
            when: !edgeDragHandler.active
            value: _control.center ? _control.center.atDistanceAndAzimuth(_control.radius, 90) : null
            restoreMode: Binding.RestoreNone
        }
        
        DragHandler {
            id: edgeDragHandler
            grabPermissions: PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType
            onActiveTranslationChanged: {
                if (active && _control.center) {
                    _control.radius = _control.center.distanceTo(edgeItem.coordinate)
                }
            }
        }
    }

	//! 获取鼠标坐标
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        enabled: _control.selected && !_control.closed
        propagateComposedEvents: true
        acceptedButtons: Qt.RightButton
        onPositionChanged: (mouse) => {
            _control._mouseCoordinate = _control.map.toCoordinate(Qt.point(mouse.x, mouse.y));
            _control.radius = _control.center ? _control.center.distanceTo(_control._mouseCoordinate) : 0;
        }
    }
}
