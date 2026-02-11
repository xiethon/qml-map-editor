import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

MapQuickItem {
    id: _control

    property var size: 10
    property var color : "blue"
	property bool selected: false

    property var _radius: size / 2
    anchorPoint.x:              _radius
    anchorPoint.y:              _radius
    sourceItem: Item {
        id:                     _item
        width:                  _control.size 
        height:                 _control.size
        Canvas {
            id:                 pointCanvas
            anchors.fill:       parent
            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)

                if(_control.selected){
                    ctx.beginPath();
                    ctx.fillStyle =  "white";
                    ctx.arc(_control._radius , _control._radius , _control._radius , 0, Math.PI * 2);
                    ctx.fill();
                }
                //! 绘制小圈
                ctx.beginPath();
                ctx.fillStyle =  _control.color;
                ctx.arc(_control._radius , _control._radius , _control._radius * 0.6 , 0, Math.PI * 2);
                ctx.fill();
            }
        }
    }
    onSelectedChanged: {
        pointCanvas.requestPaint();
    }
}

