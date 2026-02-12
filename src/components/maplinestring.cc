#include "maplinestring.h"
#include "mappoint.h"
#include "mapgeometry.h"

MapLineString::MapLineString(QObject* parent) : MapGeometry(MapGeometry::GeometryType::LineString, parent) {
    setClosed(false);
    setValid(false);
    setHasChildren(true);
}

QGeoPath MapLineString::path() const {
    QGeoPath path;
    for (const auto& point : _points) {
        path.addCoordinate(point->coordinate());
    }
    return path;
}

QVariantList MapLineString::mapPoints() const {
    QVariantList list;
    for (const auto& point : _points) {
        list.append(QVariant::fromValue(point));
    }
    return list;
}

bool MapLineString::valid() const {
    return _points.size() >= 2;
}

//! 获取children
QList<MapGeometry*> MapLineString::children() const {
    QList<MapGeometry*> list;
    for (const auto& point : _points) {
        list.append(point);
    }
    return list;
}

//! 删除选中子项
void MapLineString::removeSelectedChild() {
    auto it = _points.begin();
    while (it != _points.end()) {
        if ((*it)->selected()) {
            (*it)->deleteLater();
            it = _points.erase(it);
            emit pathChanged();
            emit validChanged();
        } else {
            ++it;
        }
    }
}

void MapLineString::appendChild(QGeoCoordinate coordinate) {
    auto point = new MapPoint(coordinate, this);

    //! 如果已经闭合则在离coordinate最近的边中间插入. 否则在尾部插入
    if (!closed()) {
        _points.append(point);
    } else {
        int insert_index = 1;
        double min_distance = std::numeric_limits<double>::max();
        for (int i = 0; i < _points.size() - 1; ++i) {
            double distance =
                MapGeometry::distanceToSegment(coordinate, _points[i]->coordinate(), _points[i + 1]->coordinate());
            if (distance < min_distance) {
                min_distance = distance;
                insert_index = i + 1;
            }
        }
        _points.insert(insert_index, point);
    }
    emit pathChanged();
    emit validChanged();
}