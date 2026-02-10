import QtQuick
import QtLocation
import QtPositioning

import map.editor
import map.editor.components

Map {
    id:map
    plugin: Plugin {
        id: taindituPlugin
        name: "Tianditu"
        PluginParameter {
            name: "Tianditu.token"
            value: "d4359a63f985f33102eb7b257170dbc0" //! 请替换为您申请的天地图密钥，注册地址：http://lbs.tianditu.gov.cn
        }
    }
    maximumZoomLevel:18
    layer.enabled: true
    layer.samples: 8

    zoomLevel: 12
    center: QtPositioning.coordinate(39.908828,116.397501) //! 北京天安门

    PinchHandler {
        id: pinch
        target: null
        onScaleChanged: (delta) => {
            map.zoomLevel += Math.log2(delta)
        }
        onRotationChanged: (delta) => {
            map.bearing -= delta
        }
        grabPermissions: PointerHandler.TakeOverForbidden
    }
    WheelHandler {
        id: wheel
        acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                            ? PointerDevice.Mouse | PointerDevice.TouchPad
                            : PointerDevice.Mouse
        rotationScale: 1/120
        property: "zoomLevel"
    }
    DragHandler {
        id: drag
        target: null
        onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
    }

    Component.onCompleted: {
        setActiveMap("Tianditu", MapType.SatelliteMapDay); //! 卫星影像图
    }

    function setActiveMap(name,style) {
        for (var i = 0; i < map.supportedMapTypes.length; i++) {
            if (name === map.supportedMapTypes[i].name && style === map.supportedMapTypes[i].style) {
                map.activeMapType = map.supportedMapTypes[i];
                return;
            }
        }
    }
}