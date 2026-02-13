#include "mapcircle.h"

MapCircle::MapCircle(QObject* parent) : MapGeometry(MapGeometry::GeometryType::Circle, parent) {
    setClosed(false);
    setValid(true);
    setHasChildren(false);
}

MapCircle::MapCircle(QGeoCoordinate center, QObject* parent) :
    MapGeometry(MapGeometry::GeometryType::Circle, parent),
    _center(center) {
    setClosed(false);
    setValid(true);
    setHasChildren(false);
}

MapCircle::MapCircle(QGeoCoordinate center, double radius, QObject* parent) :
    MapGeometry(MapGeometry::GeometryType::Circle, parent),
    _center(center),
    _radius(radius) {
    setClosed(false);
    setValid(true);
    setHasChildren(false);
}

double MapCircle::radius() const {
    return _radius;
}

void MapCircle::setRadius(double radius) {
    if (_radius != radius) {
        _radius = radius;
        emit radiusChanged();
        emit validChanged();
    }
}

QGeoCoordinate MapCircle::center() const {
    return _center;
}

void MapCircle::setCenter(const QGeoCoordinate& center) {
    if (_center != center) {
        _center = center;
        emit centerChanged();
    }
}