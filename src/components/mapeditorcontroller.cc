#include "mapeditorcontroller.h"
#include "mappoint.h"
#include "mappolygon.h"
#include "maplinestring.h"
#include "mapcircle.h"
#include "mapsquare.h"

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

//! 获取所有线
QVariantList MapEditorController::mapLineStrings() const {
    QVariantList list;
    for (const auto& linestring : _map_linestrings) {
        list.append(QVariant::fromValue(linestring));
    }
    return list;
}

//! 获取所有圆
QVariantList MapEditorController::mapCircles() const {
    QVariantList list;
    for (const auto& circle : _map_circles) {
        list.append(QVariant::fromValue(circle));
    }
    return list;
}

//! 获取所有正方形
QVariantList MapEditorController::mapSquares() const {
    QVariantList list;
    for (const auto& square : _map_squares) {
        list.append(QVariant::fromValue(square));
    }
    return list;
}

//! 删除所有数据
void MapEditorController::removeAll() {
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

    for (MapLineString* linestring : _map_linestrings) {
        linestring->deleteLater();
    }
    _map_linestrings.clear();
    emit mapLineStringsChanged();

    for (MapCircle* circle : _map_circles) {
        circle->deleteLater();
    }
    _map_circles.clear();
    emit mapCirclesChanged();
    for (MapSquare* square : _map_squares) {
        square->deleteLater();
    }
    _map_squares.clear();
    emit mapSquaresChanged();
}

//! 删除选中数据
void MapEditorController::deleteSelected() {
    auto removeSelected = [](auto& list, auto&& changedSignal) {
        auto it = list.begin();
        bool changed = false;
        while (it != list.end()) {
            if ((*it)->selected()) {
                (*it)->deleteLater();
                it = list.erase(it);
                changed = true;
            } else if ((*it)->hasChildren()) {
                (*it)->removeSelectedChild();
                if (!(*it)->valid()) {
                    (*it)->deleteLater();
                    it = list.erase(it);
                    changed = true;
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
        if (changed) {
            changedSignal();
        }
    };

    switch (_edit_mode) {
        case EditMode::MARKER:
            removeSelected(_map_markers, [this]() { emit mapMarkersChanged(); });
            break;
        case EditMode::LINESTRING:
            removeSelected(_map_linestrings, [this]() { emit mapLineStringsChanged(); });
            break;
        case EditMode::POLYGON:
            removeSelected(_map_polygons, [this]() { emit mapPolygonsChanged(); });
            break;
        case EditMode::CIRCLE:
            removeSelected(_map_circles, [this]() { emit mapCirclesChanged(); });
            break;
        case EditMode::SQUARE:
            removeSelected(_map_squares, [this]() { emit mapSquaresChanged(); });
            break;
        default:
            break;
    }
}

//! 清除当前编辑模式下所有选中状态
void MapEditorController::clearAllSelectedForMode(EditMode mode) {
    auto geometrys = getMapGeometrysByEditMode(mode);
    for (auto& geometry : geometrys) {
        geometry->setSelected(false);
    }
}

//! 清除所有选中状态
void MapEditorController::clearAllSelected() {
    clearAllSelectedForMode(EditMode::MARKER);
    clearAllSelectedForMode(EditMode::LINESTRING);
    clearAllSelectedForMode(EditMode::POLYGON);
    clearAllSelectedForMode(EditMode::CIRCLE);
    clearAllSelectedForMode(EditMode::SQUARE);
}

//! 设置选中项，并清除当前编辑模式下其他项的选中状态
void MapEditorController::setSelectedItemAndClearOthers(const QString& uuid) {
    clearAllSelected();
    auto geometrys = getMapGeometrysByEditMode(_edit_mode);
    for (auto& geometry : geometrys) {
        if (geometry->uuid() == uuid) {
            geometry->setSelected(true);
            return;
        } else {
            if (geometry->hasChildren()) {
                auto children = geometry->children();
                for (auto& child : children) {
                    if (child->uuid() == uuid) {
                        child->setSelected(true);
                        return;
                    }
                }
            }
        }
    }
}

//! 设置选中项
void MapEditorController::setSelectedItem(const QString& uuid) {
    auto geometrys = getMapGeometrysByEditMode(_edit_mode);
    for (auto& geometry : geometrys) {
        if (geometry->uuid() == uuid) {
            geometry->setSelected(true);
            return;
        } else {
            if (geometry->hasChildren()) {
                auto children = geometry->children();
                for (auto& child : children) {
                    if (child->uuid() == uuid) {
                        child->setSelected(true);
                        return;
                    }
                }
            }
        }
    }
}

//! 添加操作
void MapEditorController::append(QGeoCoordinate coordinate) {
    auto geometrys = getMapGeometrysByEditMode(_edit_mode);
    bool appended = false;
    for (auto& geometry : geometrys) {
        if (geometry->hasChildren() && geometry->selected()) {
            geometry->appendChild(coordinate);
            appended = true;
            sendGeometryChangedSignal(_edit_mode);
            break;
        }
    }
    if (!appended) {
        clearAllSelected();
        switch (_edit_mode) {
            case EditMode::MARKER: {
                _map_markers.append(new MapPoint(coordinate, this));
                emit mapMarkersChanged();
                break;
            }
            case EditMode::LINESTRING: {
                auto linestring = new MapLineString(this);
                linestring->appendChild(coordinate);
                _map_linestrings.append(linestring);
                emit mapLineStringsChanged();
                break;
            }
            case EditMode::POLYGON: {
                auto polygon = new MapPolygon(this);
                polygon->appendChild(coordinate);
                _map_polygons.append(polygon);
                emit mapPolygonsChanged();
                break;
            }
            case EditMode::CIRCLE: {
                _map_circles.append(new MapCircle(coordinate, this));
                emit mapCirclesChanged();
                break;
            }
            case EditMode::SQUARE: {
                _map_squares.append(new MapSquare(coordinate, this));
                emit mapSquaresChanged();
                break;
            }
            default:
                break;
        }
    }
}

//! 结束当前当前编辑对象的操作
void MapEditorController::finishCurrentEditGeometry() {
    auto finishEdit = [](auto& list, auto&& changedSignal) {
        auto it = list.begin();
        bool changed = false;
        //! 如果当前编辑对象未闭合: 如果是valid则闭合，否则删除
        while (it != list.end()) {
            if ((*it)->selected()) {
                if (!(*it)->closed()) {
                    if ((*it)->valid()) {
                        (*it)->setClosed(true);
                    } else {
                        (*it)->deleteLater();
                        it = list.erase(it);
                        changed = true;
                        continue;
                    }
                }
            }
            ++it;
        }
        if (changed) {
            changedSignal();
        }
    };

    switch (_edit_mode) {
        case EditMode::MARKER:
            finishEdit(_map_markers, [this]() { emit mapMarkersChanged(); });
            break;
        case EditMode::LINESTRING:
            finishEdit(_map_linestrings, [this]() { emit mapLineStringsChanged(); });
            break;
        case EditMode::POLYGON:
            finishEdit(_map_polygons, [this]() { emit mapPolygonsChanged(); });
            break;
        case EditMode::CIRCLE:
            finishEdit(_map_circles, [this]() { emit mapCirclesChanged(); });
            break;
        case EditMode::SQUARE:
            finishEdit(_map_squares, [this]() { emit mapSquaresChanged(); });
            break;
        default:
            break;
    }
    clearAllSelected();
}

QList<MapGeometry*> MapEditorController::getMapGeometrysByEditMode(const EditMode& mode) {
    QList<MapGeometry*> list;
    switch (mode) {
        case EditMode::MARKER:
            for (const auto& point : _map_markers) {
                list.append(point);
            }
            break;
        case EditMode::LINESTRING:
            for (const auto& linestring : _map_linestrings) {
                list.append(linestring);
            }
            break;
        case EditMode::POLYGON:
            for (const auto& polygon : _map_polygons) {
                list.append(polygon);
            }
            break;
        case EditMode::CIRCLE:
            for (const auto& circle : _map_circles) {
                list.append(circle);
            }
            break;
        case EditMode::SQUARE:
            for (const auto& square : _map_squares) {
                list.append(square);
            }
            break;
        default:
            break;
    }
    return list;
}

void MapEditorController::sendGeometryChangedSignal(const EditMode& modeEditor) {
    switch (modeEditor) {
        case EditMode::MARKER:
            emit mapMarkersChanged();
            break;
        case EditMode::LINESTRING:
            emit mapLineStringsChanged();
            break;
        case EditMode::POLYGON:
            emit mapPolygonsChanged();
            break;
        case EditMode::CIRCLE:
            emit mapCirclesChanged();
            break;
        case EditMode::SQUARE:
            emit mapSquaresChanged();
            break;
        default:
            break;
    }
}