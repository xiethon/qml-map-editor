#include "maplinestring.h"
#include "mappoint.h"

MapLineString::MapLineString(QObject* parent) : MapGeometry(parent) {}

QGeoPath MapLineString::path() const {
    QGeoPath path;
    for (const auto& point : _points) {
        path.addCoordinate(point->coordinate());
    }
    return path;
}

QVariantList MapLineString::points() const {
    QVariantList list;
    for (const auto& point : _points) {
        list.append(QVariant::fromValue(point));
    }
    return list;
}

int MapLineString::size() const {
    return _points.size();
}

void MapLineString::append(QGeoCoordinate coordinate) {
    _points.append(new MapPoint(coordinate, this));
    emit pathChanged();
    emit sizeChanged();
}
