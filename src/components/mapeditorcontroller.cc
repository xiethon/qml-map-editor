#include "mapeditorcontroller.h"
#include "mappoint.h"
#include "mappolygon.h"
// #include "maplinestring.h"

MapEditorController::MapEditorController(QObject* parent) : QObject(parent) {}

MapEditorController::EditMode MapEditorController::editMode() const {
    return _edit_mode;
}

void MapEditorController::setEditMode(EditMode mode) {
    if (_edit_mode != mode) {
        clearAllSelectedForMode(_edit_mode);
        _edit_mode = mode;
        emit editModeChanged();
    }
}

QVariantList MapEditorController::mapPoints() const {
    QVariantList list;
    for (const auto& point : _map_points) {
        list.append(QVariant::fromValue(point));
    }
    return list;
}

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

void MapEditorController::append(QGeoCoordinate coordinate) {
    switch (_edit_mode) {
        case EditMode::POINT: {
            clearAllSelectedForMode(EditMode::POINT);
            MapPoint* point = new MapPoint(coordinate, this);
            _map_points.append(point);
            emit mapPointsChanged();
            break;
        }
        case EditMode::LINESTRING:
            break;
        case EditMode::POLYGON:
            if (_map_polygons.isEmpty()) {
                MapPolygon* polygon = new MapPolygon(this);
                polygon->append(coordinate);
                _map_polygons.append(polygon);
                emit mapPolygonsChanged();
            } else {
                _map_polygons.last()->append(coordinate);
            }
            break;
        default:
            break;
    }
    emit canCloseChanged();
}

void MapEditorController::clearAll() {
    for (MapPoint* point : _map_points) {
        point->deleteLater();
    }
    _map_points.clear();
    emit mapPointsChanged();
}

void MapEditorController::deleteSelected() {
    switch (_edit_mode) {
        case EditMode::POINT: {
            auto it = _map_points.begin();
            while (it != _map_points.end()) {
                if ((*it)->selected()) {
                    (*it)->deleteLater();
                    it = _map_points.erase(it);
                } else {
                    ++it;
                }
            }
            emit mapPointsChanged();
            break;
        }
        default:
            break;
    }
}

void MapEditorController::closeCurrentGeometry() {
    // todo: 实现自动闭合逻辑
}

bool MapEditorController::locked() const {
    return _locked;
}

void MapEditorController::setLocked(bool locked) {
    if (_locked != locked) {
        _locked = locked;
        emit lockedChanged();
    }
}

bool MapEditorController::canClose() const {
    bool can_close = false;
    switch (_edit_mode) {
        case EditMode::POINT:
            for (MapPoint* point : _map_points) {
                if (point->selected() && point->canClose() && !point->closed()) {
                    can_close = true;
                    break;
                }
            }
            break;
        case EditMode::LINESTRING:
            break;
        case EditMode::POLYGON:
            for (MapPolygon* polygon : _map_polygons) {
                if (polygon->selected() && polygon->canClose() && !polygon->closed()) {
                    can_close = true;
                    break;
                }
            }
            break;
        default:
            break;
    }
    return can_close;
}

void MapEditorController::clearAllSelectedForMode(EditMode mode) {
    switch (mode) {
        case EditMode::POINT:
            for (MapPoint* point : _map_points) {
                if (point->selected()) {
                    point->setSelected(false);
                }
            }
            break;
        default:
            break;
    }
}

void MapEditorController::clearAllSelected() {
    clearAllSelectedForMode(EditMode::POINT);
    clearAllSelectedForMode(EditMode::LINESTRING);
    clearAllSelectedForMode(EditMode::POLYGON);
}

void MapEditorController::setSelectedItemAndClearOthers(int item_id) {
    clearAllSelected();
    for (const auto& point : _map_points) {
        if (point->id() == item_id) {
            point->setSelected(true);
            return;
        }
    }
}

void MapEditorController::setSelectedItem(int item_id) {
    for (const auto& point : _map_points) {
        if (point->id() == item_id) {
            point->setSelected(true);
            return;
        }
    }
}