#include "mapeditorcontroller.h"
#include "mappoint.h"
#include "mappolygon.h"
// #include "maplinestring.h"

MapEditorController::MapEditorController(QObject* parent) : QObject(parent) {}

//! 获取编辑模式
MapEditorController::EditMode MapEditorController::editMode() const {
    return _edit_mode;
}

//! 设置编辑模式
void MapEditorController::setEditMode(EditMode mode) {
    if (_edit_mode != mode) {
        clearAllSelectedForMode(_edit_mode);
        _edit_mode = mode;
        emit editModeChanged();
    }
}

//! 获取编辑锁状态
bool MapEditorController::locked() const {
    return _locked;
}

//! 设置编辑锁状态
void MapEditorController::setLocked(bool locked) {
    if (_locked != locked) {
        _locked = locked;
        emit lockedChanged();
    }
}

//! 获取所有标记点
QVariantList MapEditorController::mapMarkers() const {
    QVariantList list;
    for (const auto& point : _map_markers) {
        list.append(QVariant::fromValue(point));
    }
    return list;
}

//! 获取所有多边形
QVariantList MapEditorController::mapPolygons() const {
    QVariantList list;
    for (const auto& polygon : _map_polygons) {
        list.append(QVariant::fromValue(polygon));
    }
    return list;
}
// QVariantList MapEditorController::mapLineStrings() const {
//     QVariantList list;
//     for (const auto& line : _map_linestrings) {
//         list.append(QVariant::fromValue(line));
//     }
//     if (_current_linestring) {
//         list.append(QVariant::fromValue(_current_linestring));
//     }
//     return list;
// }

//! 删除所有数据
void MapEditorController::clearAll() {
    for (MapPoint* point : _map_markers) {
        point->deleteLater();
    }
    _map_markers.clear();
    emit mapMarkersChanged();

    for (MapPolygon* polygon : _map_polygons) {
        polygon->deleteLater();
    }
    _map_polygons.clear();
    emit mapPolygonsChanged();
}

//! 删除选中数据
void MapEditorController::deleteSelected() {
    switch (_edit_mode) {
        case EditMode::MARKER: {
            auto it = _map_markers.begin();
            while (it != _map_markers.end()) {
                if ((*it)->selected()) {
                    (*it)->deleteLater();
                    it = _map_markers.erase(it);
                } else {
                    ++it;
                }
            }
            emit mapMarkersChanged();
            break;
        }
        case EditMode::LINESTRING:
            break;
        case EditMode::POLYGON: {
            auto it = _map_polygons.begin();
            while (it != _map_polygons.end()) {
                if ((*it)->selected()) {
                    (*it)->deleteLater();
                    it = _map_polygons.erase(it);
                } else {
                    ++it;
                }
            }
            emit mapPolygonsChanged();
            break;
        }
        default:
            break;
    }
}

//! 清除当前编辑模式下所有选中状态
void MapEditorController::clearAllSelectedForMode(EditMode mode) {
    switch (mode) {
        case EditMode::MARKER:
            for (MapPoint* point : _map_markers) {
                if (point->selected()) {
                    point->setSelected(false);
                }
            }
            break;
        case EditMode::LINESTRING:
            break;
        case EditMode::POLYGON:
            for (MapPolygon* polygon : _map_polygons) {
                polygon->setSelected(false);
            }
            break;
        default:
            break;
    }
}

//! 清除所有选中状态
void MapEditorController::clearAllSelected() {
    clearAllSelectedForMode(EditMode::MARKER);
    clearAllSelectedForMode(EditMode::LINESTRING);
    clearAllSelectedForMode(EditMode::POLYGON);
}

//! 设置选中项，并清除当前编辑模式下其他项的选中状态
void MapEditorController::setSelectedItemAndClearOthers(const QString& uuid) {
    clearAllSelected();
    switch (_edit_mode) {
        case EditMode::MARKER: {
            for (const auto& point : _map_markers) {
                if (point->uuid() == uuid) {
                    point->setSelected(true);
                    return;
                }
            }
            break;
        }
        case EditMode::LINESTRING:
            break;
        case EditMode::POLYGON: {
            for (const auto& polygon : _map_polygons) {
                if (polygon->uuid() == uuid) {
                    polygon->setSelected(true);
                    return;
                }
            }
            break;
        }
        default:
            break;
    }
}

//! 设置选中项
void MapEditorController::setSelectedItem(const QString& uuid) {
    for (const auto& point : _map_markers) {
        if (point->uuid() == uuid) {
            point->setSelected(true);
            return;
        }
    }
}

//! 添加操作
void MapEditorController::append(QGeoCoordinate coordinate) {
    switch (_edit_mode) {
        case EditMode::MARKER: {
            clearAllSelectedForMode(EditMode::MARKER);
            MapPoint* point = new MapPoint(coordinate, this);
            _map_markers.append(point);
            emit mapMarkersChanged();
            break;
        }
        case EditMode::LINESTRING:
            break;
        case EditMode::POLYGON: {
            bool appended = false;
            for (auto& polygon : _map_polygons) {
                if (polygon->selected()) {
                    polygon->append(coordinate);
                    appended = true;
                    emit mapPolygonsChanged();
                    break;
                }
            }
            if (!appended) {
                clearAllSelected();
                MapPolygon* polygon = new MapPolygon(this);
                polygon->append(coordinate);
                _map_polygons.append(polygon);
                emit mapPolygonsChanged();
            }
            break;
        }
        default:
            break;
    }
}

//! 结束当前当前编辑对象的操作
void MapEditorController::finishCurrentEditing() {
    switch (_edit_mode) {
        case EditMode::MARKER:
            break;
        case EditMode::LINESTRING:
            break;
        case EditMode::POLYGON: {
            //! 如果多边形不是有效的则删除未闭合的多边形，如果当前编辑的多边形未闭合且是有效的，则进行闭合
            auto it = _map_polygons.begin();
            while (it != _map_polygons.end()) {
                if (!(*it)->valid()) {
                    (*it)->deleteLater();
                    it = _map_polygons.erase(it);
                } else {
                    if ((*it)->selected() && !(*it)->closed() && (*it)->valid()) {
                        (*it)->closeGeometry();
                    }
                    ++it;
                }
            }
            emit mapPolygonsChanged();
            break;
        }
        default:
            break;
    }
    clearAllSelected();
}