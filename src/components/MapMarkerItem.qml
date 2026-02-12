//! MapPointItem.qml

import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Effects

MapQuickItem {
    id: _control
    
	property alias source: sourceImg.source
	property var address: ""
    property bool selected: false
	property bool _showAddress: selected && address !== ""

	anchorPoint.x: _control.sourceSize / 2
    anchorPoint.y: (_showAddress ? 22 : 0) + _control.sourceSize
    property var sourceSize: 24
    property var color: "white"

	signal dragFinished()	//! 拖动结束信号

    HoverHandler {
        id: hoverHandler
    }

    DragHandler {
        id: dragHandler
        grabPermissions: PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType
		onActiveChanged: {
			if (!active) {
				_control.dragFinished()
			}
		}
    }

    sourceItem: Item {
		implicitWidth:  Math.max(_control.sourceSize, _control._showAddress ? addressLabel.width : 0)
		implicitHeight: _control.sourceSize + (_control._showAddress ? 22 : 0)
		Image {
			id: sourceImg
			x: 0
			y: _control._showAddress ? 22 : 0
			visible: false
			sourceSize: Qt.size(_control.sourceSize, _control.sourceSize)
			fillMode: Image.PreserveAspectFit
			smooth: true
			asynchronous: true
		}

		MultiEffect {
			anchors.fill: sourceImg
			source: sourceImg
			brightness: 1.0
			colorization: 1.0
			colorizationColor: _control.color
			opacity: _control.enabled ? 1.0 : 0.6
			visible: _control.source !== ""
		}

		Rectangle {
			id: addressLabel
			visible: _control._showAddress
			x: 0
			y: 0
			color: "#CC000000"
			radius: 4
			width: addressText.width + 8
			height: addressText.height + 4
			
			Text {
				id: addressText
				anchors.centerIn: parent
				text: _control.address
				color: "white"
				font.pixelSize: 11
			}
		}
    }
}

