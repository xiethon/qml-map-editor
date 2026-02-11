#include "mappolygon.h"
#include "mappoint.h"
#include <limits>

namespace {
double distanceToSegment(const QGeoCoordinate& point, const QGeoCoordinate& start, const QGeoCoordinate& end) {
    double x0 = point.longitude();
    double y0 = point.latitude();
    double x1 = start.longitude();
    double y1 = start.latitude();
    double x2 = end.longitude();
    double y2 = end.latitude();

    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx == 0 && dy == 0) {
        dx = x0 - x1;
        dy = y0 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }
    double t = ((x0 - x1) * dx + (y0 - y1) * dy) / (dx * dx + dy * dy);
    t = std::max(0.0, std::min(1.0, t));
    double projX = x1 + t * dx;
    double projY = y1 + t * dy;
    dx = x0 - projX;
    dy = y0 - projY;
    return std::sqrt(dx * dx + dy * dy);
}
} // namespace

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
        //! 闭合后，找到离coordinate最近的边，在该边中间插入
        int insert_index = 1;
        double min_distance = std::numeric_limits<double>::max();

        for (int i = 0; i < _points.size(); ++i) {
            int next_index = (i + 1) % _points.size();
            double distance =
                distanceToSegment(coordinate, _points[i]->coordinate(), _points[next_index]->coordinate());
            if (distance < min_distance) {
                min_distance = distance;
                insert_index = i + 1;
            }
        }

        _points.insert(insert_index, new MapPoint(coordinate, this));
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