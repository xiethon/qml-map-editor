import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Effects

MapItemGroup {
    id: _control

    property var map

    property var color : "blue"        //! 填充颜色
    property var borderColor : "blue"  //! 边颜色

    property bool selected: false      //! 是否选中
    property bool closed: false        //! 是否闭合
    property var center: null          //! 中心坐标
    property var sideLength: 0         //! 边长
     
    property var _mouseCoordinate: null //! 鼠标坐标

    signal squareClicked()             //! 设置选中正方形信号

    //! 计算四个角点
    function _topLeft() {
        if (!center || sideLength <= 0) return null;
        var halfSide = sideLength / 2.0;
        var top = center.atDistanceAndAzimuth(halfSide, 0);
        return top.atDistanceAndAzimuth(halfSide, 270);
    }
    
    function _topRight() {
        if (!center || sideLength <= 0) return null;
        var halfSide = sideLength / 2.0;
        var top = center.atDistanceAndAzimuth(halfSide, 0);
        return top.atDistanceAndAzimuth(halfSide, 90);
    }
    
    function _bottomRight() {
        if (!center || sideLength <= 0) return null;
        var halfSide = sideLength / 2.0;
        var bottom = center.atDistanceAndAzimuth(halfSide, 180);
        return bottom.atDistanceAndAzimuth(halfSide, 90);
    }
    
    function _bottomLeft() {
        if (!center || sideLength <= 0) return null;
        var halfSide = sideLength / 2.0;
        var bottom = center.atDistanceAndAzimuth(halfSide, 180);
        return bottom.atDistanceAndAzimuth(halfSide, 270);
    }

    //! 正方形多边形
    MapPolygon {
        id: squareItem
        visible: _control.center !== null && _control.sideLength > 0
        color: _control.color
        border.width: 2
        border.color: _control.borderColor
        opacity: 0.4
        path: {
            var tl = _topLeft();
            var tr = _topRight();
            var br = _bottomRight();
            var bl = _bottomLeft();
            if (tl && tr && br && bl) {
                return [tl, tr, br, bl];
            }
            return [];
        }
        TapHandler {
            gesturePolicy: TapHandler.ReleaseWithinBounds
            enabled: !_control.selected
            onTapped: squareClicked();
        }
    }

    //! visual line (从中心到鼠标或边缘控制点)
    MapSegmentItem {
        id: dashItem
        map: _control.map
        visible: _control.selected
        start: _control.center
        end: _control.closed ? edgeItem.coordinate : _control._mouseCoordinate
        color: _control.borderColor
        solid: false
        selected: true
    }

    //! 中心点
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

    //! 边缘控制点（右侧中点）
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
            value: _control.center ? _control.center.atDistanceAndAzimuth(_control.sideLength / 2, 90) : null
            restoreMode: Binding.RestoreNone
        }
        
        DragHandler {
            id: edgeDragHandler
            grabPermissions: PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType
            onActiveTranslationChanged: {
                if (active && _control.center) {
                    // 边长 = 中心到边缘点距离 * 2
                    _control.sideLength = _control.center.distanceTo(edgeItem.coordinate) * 2
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
            // 边长 = 中心到鼠标距离 * 2
            _control.sideLength = _control.center ? _control.center.distanceTo(_control._mouseCoordinate) * 2 : 0;
        }
    }
}
