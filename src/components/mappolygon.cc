#include "mappolygon.h"
#include "mappoint.h"
#include <limits>

MapPolygon::MapPolygon(QObject* parent) : MapGeometry(MapGeometry::GeometryType::Polygon, parent) {
    setClosed(false);
    setValid(false);
    setHasChildren(true);
}

//! 获取geopath路径
QGeoPath MapPolygon::path() const {
    QGeoPath path;
    for (const auto& point : _points) {
        path.addCoordinate(point->coordinate());
    }
    return path;
}

//! 插入一个点
void MapPolygon::appendChild(QGeoCoordinate coordinate) {
    setAllChildrenSelected(false);

    if (!closed() || _points.size() < 2) {
        //! 未闭合时，在尾部插入
        _points.append(new MapPoint(coordinate, this));
    } else {
        //! 闭合后，找到离coordinate最近的边，在该边中间插入
        int insert_index = 1;
        double min_distance = std::numeric_limits<double>::max();

        for (int i = 0; i < _points.size(); ++i) {
            int next_index = (i + 1) % _points.size();
            double distance =
                MapGeometry::distanceToSegment(coordinate, _points[i]->coordinate(), _points[next_index]->coordinate());
            if (distance < min_distance) {
                min_distance = distance;
                insert_index = i + 1;
            }
        }

        _points.insert(insert_index, new MapPoint(coordinate, this));
    }

    emit pathChanged();
    emit validChanged();
}

//! 是否有效的多边形
bool MapPolygon::valid() const {
    return _points.size() >= 3;
}

//! 获取所有点
QVariantList MapPolygon::mapPoints() const {
    QVariantList list;
    for (const auto& point : _points) {
        list.append(QVariant::fromValue(point));
    }
    return list;
}

//! 获取children
QList<MapGeometry*> MapPolygon::children() const {
    QList<MapGeometry*> list;
    for (const auto& point : _points) {
        list.append(point);
    }
    return list;
}

void MapPolygon::removeSelectedChild() {
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