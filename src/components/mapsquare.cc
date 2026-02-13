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

QGeoCoordinate MapSquare::topLeft() const {
    if (!_center.isValid() || _side_length <= 0) {
        return QGeoCoordinate();
    }
    // 从中心向左上角移动：先向上（北）移动半边长，再向左（西）移动半边长
    double halfSide = _side_length / 2.0;
    QGeoCoordinate top = _center.atDistanceAndAzimuth(halfSide, 0); // 向北
    return top.atDistanceAndAzimuth(halfSide, 270); // 向西
}

QGeoCoordinate MapSquare::topRight() const {
    if (!_center.isValid() || _side_length <= 0) {
        return QGeoCoordinate();
    }
    double halfSide = _side_length / 2.0;
    QGeoCoordinate top = _center.atDistanceAndAzimuth(halfSide, 0); // 向北
    return top.atDistanceAndAzimuth(halfSide, 90); // 向东
}

QGeoCoordinate MapSquare::bottomRight() const {
    if (!_center.isValid() || _side_length <= 0) {
        return QGeoCoordinate();
    }
    double halfSide = _side_length / 2.0;
    QGeoCoordinate bottom = _center.atDistanceAndAzimuth(halfSide, 180); // 向南
    return bottom.atDistanceAndAzimuth(halfSide, 90); // 向东
}

QGeoCoordinate MapSquare::bottomLeft() const {
    if (!_center.isValid() || _side_length <= 0) {
        return QGeoCoordinate();
    }
    double halfSide = _side_length / 2.0;
    QGeoCoordinate bottom = _center.atDistanceAndAzimuth(halfSide, 180); // 向南
    return bottom.atDistanceAndAzimuth(halfSide, 270); // 向西
}
