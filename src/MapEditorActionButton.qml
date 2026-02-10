import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Templates as T

T.Button {
	id: control

	property alias source: sourceImg.source
	property color iconColor: "white"
	property var toolTipText: ""
	property color backgroundColor: "#232425"
	property color backgroundHoverColor: "#2c2c2c"
	property color backgroundPressedColor: "#1f1f1f"
	property color backgroundDisabledColor: "#b5b9c6"

	implicitWidth: 36
	implicitHeight: 36
    
    property int iconSize: implicitWidth * 0.6

	hoverEnabled: true
	focusPolicy: Qt.NoFocus

	background: Rectangle {
		radius: 4
		color: control.enabled
			   ? (control.down ? control.backgroundPressedColor
							   : (control.hovered ? control.backgroundHoverColor
												  : control.backgroundColor))
			   : control.backgroundDisabledColor
	}

	contentItem: Item {
		implicitWidth: control.iconSize
		implicitHeight: control.iconSize

		Image {
			id: sourceImg
			anchors.centerIn: parent
			visible: false
			source: control.iconSource
			sourceSize: Qt.size(control.iconSize, control.iconSize)
			fillMode: Image.PreserveAspectFit
			smooth: true
			asynchronous: true
		}

		MultiEffect {
			anchors.fill: sourceImg
			source: sourceImg
			brightness: 1.0
			colorization: 1.0
			colorizationColor: control.iconColor
			opacity: control.enabled ? 1.0 : 0.6
			visible: control.iconSource !== ""
		}
	}

	ToolTip.text: control.toolTipText
    ToolTip.delay: 500
    ToolTip.timeout: 3000
    ToolTip.visible:  control.toolTipText !== "" && hovered
}

