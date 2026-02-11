import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

MapItemGroup{
    id: _control

    property var start
    property var end

    property var color
    property var map
    property bool selected: true
    property bool solid: true

    property var zoomLevel_: map.zoomLevel

    MapQuickItem{
        coordinate: midCoordinate()
        visible: segmentLength() > 10
        anchorPoint.x: lineCanvas.width / 2
        anchorPoint.y: lineCanvas.height / 2
        sourceItem: Canvas {
            id: lineCanvas
            width: segmentLength()
            height: 3
            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.strokeStyle = _control.color;
                ctx.lineWidth = 3;
                ctx.beginPath();
                ctx.moveTo(0, 0);
                ctx.lineTo(width, 0);
                if (!_control.solid) {
                    ctx.setLineDash([5, 5]);
                }
                ctx.stroke();
            }
        }
        rotation: rotationAngle()
    }

    MapQuickItem{
        coordinate: midCoordinate()
        anchorPoint.x: distanceLabel.width / 2
        anchorPoint.y: distanceLabel.height/2
        visible: _control.selected && lineCanvas.width > 10
        sourceItem: Label {
            id: distanceLabel
            text: distance().toFixed(1) + " m"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }
    }

    function midCoordinate() {
        if(start === null || end === null ){
            return null;
        }
        var lat = (start.latitude + end.latitude) / 2;
        var lon = (start.longitude + end.longitude) / 2;
        return QtPositioning.coordinate(lat, lon);
    }

    function toMapPixels(coordinate) {
        var point = map.fromCoordinate(coordinate,false);
        return point;
    }

    function segmentLength() {
        if(start === null || end === null ){
            return 0;
        }
        var startPixel = toMapPixels(start);
        var endPixel = toMapPixels(end);
        var length = Math.sqrt(Math.pow(endPixel.x - startPixel.x, 2) + Math.pow(endPixel.y - startPixel.y, 2));
        return length;
    }

    function distance(){
        if(start === null || end === null ){
            return 0;
        }
        var distance = start.distanceTo(end);
        return distance;
    }

    function rotationAngle() {
        if(start === null || end === null ){
            return 0;
        }
        var azimuth = start.azimuthTo(end);
        return azimuth - 90;
    }

    onZoomLevel_Changed: {
        lineCanvas.width = segmentLength();
        lineCanvas.requestPaint();
    }

    onEndChanged: {
        lineCanvas.width = segmentLength();
        lineCanvas.requestPaint();
    }

    onSelectedChanged: {
        lineCanvas.requestPaint();
    }
}
