import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
 
MapItemGroup{
    id: _control
 
    property var path: []
    property var color : "blue"
    property var vertexColor
    property var edgeColor
    property bool selected: false

    //! 多边形
    MapPolygon {
        id:             polygonItem
        path:           _control.path
        color:          _control.color
        opacity:        _control.selected ? 0.4 : 0.2
    }
}