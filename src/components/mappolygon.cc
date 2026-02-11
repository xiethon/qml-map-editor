#include "mappolygon.h"
#include "mappoint.h"
#include <limits>

MapPolygon::MapPolygon(QObject* parent) : MapGeometry(parent) {}

QGeoPath MapPolygon::path() const {
    QGeoPath path;
    for (const auto& point : _points) {
        path.addCoordinate(point->coordinate());
    }
    return path;
}

void MapPolygon::append(QGeoCoordinate coordinate) {
    clearAllPointSelected();

    if (!closed() || _points.size() < 2) {
        //! 未闭合时，在尾部插入
        _points.append(new MapPoint(coordinate, this));
    } else {
        //! 闭合后，先找到离coordinate最近的点
        int nearestIndex = 0;
        double minDistance = std::numeric_limits<double>::max();

        for (int i = 0; i < _points.size(); ++i) {
            double distance = _points[i]->coordinate().distanceTo(coordinate);
            if (distance < minDistance) {
                minDistance = distance;
                nearestIndex = i;
            }
        }

        //! 比较最近点的前后两个点，哪个离coordinate更近
        int prevIndex = (nearestIndex - 1 + _points.size()) % _points.size();
        int nextIndex = (nearestIndex + 1) % _points.size();

        double distToPrev = _points[prevIndex]->coordinate().distanceTo(coordinate);
        double distToNext = _points[nextIndex]->coordinate().distanceTo(coordinate);

        int insertIndex;
        if (distToPrev < distToNext) {
            //! 前一个点更近，插入到 prevIndex 和 nearestIndex 之间
            insertIndex = nearestIndex;
        } else {
            //! 后一个点更近，插入到 nearestIndex 和 nextIndex 之间
            insertIndex = nearestIndex + 1;
        }

        _points.insert(insertIndex, new MapPoint(coordinate, this));
    }

    emit pathChanged();
}

bool MapPolygon::valid() const {
    return _points.size() >= 3;
}

QVariantList MapPolygon::mapPoints() const {
    QVariantList list;
    for (const auto& point : _points) {
        list.append(QVariant::fromValue(point));
    }
    return list;
}

void MapPolygon::closeGeometry() { //! 闭合几何对象
    if (valid() && !closed()) {
        setClosed(true);
        emit pathChanged();
    }
    setSelected(false);
}

void MapPolygon::clearAllPointSelected() {
    for (const auto& point : _points) {
        point->setSelected(false);
    }
}

void MapPolygon::setSelectedPoint(const QString& uuid) {
    for (const auto& point : _points) {
        if (point->uuid() == uuid) {
            point->setSelected(true);
        } else {
            point->setSelected(false);
        }
    }
    setSelected(true);
}

void MapPolygon::setSelected(bool selected) {
    MapGeometry::setSelected(selected);
    if (!selected) {
        clearAllPointSelected();
    }
}