#include "mappolygon.h"

MapPolygon::MapPolygon(QObject* parent) : MapGeometry(parent) {
    setCanCloseCapability(false);
}

QVariantList MapPolygon::path() const {
    QVariantList list;
    for (const auto& point : _points) {
        list.append(QVariant::fromValue(point));
    }
    return list;
}

void MapPolygon::append(QGeoCoordinate coordinate) {
    _points.append(new MapPoint(coordinate, this));
    emit pathChanged();
    if (_points.size() >= 3 && !closed()) {
        setCanCloseCapability(true);
    }
}