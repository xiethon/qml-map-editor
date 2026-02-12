import QtQuick
import QtLocation
import QtPositioning

import map.editor
import map.editor.components

Map {
    id:map
    plugin: Plugin {
        id: tiandituPlugin
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

    //! 逆地理编码模型
    GeocodeModel {
        id: reverseGeocodeModel
        plugin: tiandituPlugin
        autoUpdate: false
        
        property var _callback: null
        property int _queryId: 0
        property int _processedId: -1
        
        function _handleResult() {
            if (_callback && _queryId !== _processedId) {
                _processedId = _queryId
                if (count > 0) {
                    var location = get(0)
                    _callback(location.address.text, null)
                } else {
                    _callback("", "未找到地址")
                }
                _callback = null
            }
        }
        
        onStatusChanged: {
            if (status === GeocodeModel.Ready) {
                _handleResult()
            } else if (status === GeocodeModel.Error) {
                if (_callback) {
                    _processedId = _queryId
                    _callback("", errorString)
                    _callback = null
                }
            }
        }
        
        onLocationsChanged: {
            if (status === GeocodeModel.Ready) {
                _handleResult()
            }
        }
    }

    //! 根据coordinate对象获取地址名称（回调方式）
    function reverseGeocode(coordinate, callback) {
        if (!coordinate || !coordinate.isValid) {
            if (callback) callback("", "无效的坐标")
            return
        }
        reverseGeocodeModel._queryId++
        reverseGeocodeModel._callback = callback
        reverseGeocodeModel.query = coordinate
        reverseGeocodeModel.update()
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