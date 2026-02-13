#include "mapsquare.h"

MapSquare::MapSquare(QObject* parent) : MapGeometry(MapGeometry::GeometryType::Square, parent) {
    setClosed(false);
    setValid(true);
    setHasChildren(false);
}

MapSquare::MapSquare(QGeoCoordinate center, QObject* parent) :
    MapGeometry(MapGeometry::GeometryType::Square, parent),
    _center(center) {
    setClosed(false);
    setValid(true);
    setHasChildren(false);
}

MapSquare::MapSquare(QGeoCoordinate center, double side_length, QObject* parent) :
    MapGeometry(MapGeometry::GeometryType::Square, parent),
    _center(center),
    _side_length(side_length) {
    setClosed(false);
    setValid(true);
    setHasChildren(false);
}

double MapSquare::sideLength() const {
    return _side_length;
}

void MapSquare::setSideLength(double side_length) {
    if (_side_length != side_length) {
        _side_length = side_length;
        emit sideLengthChanged();
        emit validChanged();
    }
}

QGeoCoordinate MapSquare::center() const {
    return _center;
}

void MapSquare::setCenter(const QGeoCoordinate& center) {
    if (_center != center) {
        _center = center;
        emit centerChanged();
    }
}
