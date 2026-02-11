//! MapPointItem.qml

import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import QtQuick.Effects

MapQuickItem {
    id: _control
    property alias source: sourceImg.source

    anchorPoint.x: sourceImg.width/2
    anchorPoint.y: sourceImg.height
    property var sourceSize: 24
    property var color: "white"
    
    HoverHandler {
        id: hoverHandler
    }

    DragHandler {
        id: dragHandler
        grabPermissions: PointerHandler.CanTakeOverFromItems | PointerHandler.CanTakeOverFromHandlersOfDifferentType
    }

    sourceItem: Item {
		implicitWidth:  _control.sourceSize
		implicitHeight: _control.sourceSize
		Image {
			id: sourceImg
			anchors.centerIn: parent
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
    }
}

