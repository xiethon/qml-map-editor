#include "mappoint.h"

MapPoint::MapPoint(QObject* parent) : MapGeometry(parent) {
    setClosed(true);
}

MapPoint::MapPoint(const QGeoCoordinate& coordinate, QObject* parent) : MapGeometry(parent), _coordinate(coordinate) {
    setClosed(true);
}

MapPoint::MapPoint(double latitude, double longitude, QObject* parent) :
    MapGeometry(parent),
    _coordinate(latitude, longitude) {
    setClosed(true);
}

bool MapPoint::operator==(const MapPoint& other) const {
    return qFuzzyCompare(_coordinate.latitude(), other._coordinate.latitude()) &&
           qFuzzyCompare(_coordinate.longitude(), other._coordinate.longitude());
}

QGeoCoordinate MapPoint::coordinate() const {
    return _coordinate;
}

void MapPoint::setCoordinate(const QGeoCoordinate& coordinate) {
    if (_coordinate == coordinate) {
        return;
    }
    _coordinate = coordinate;
    emit coordinateChanged();
}

double MapPoint::latitude() const {
    return _coordinate.latitude();
}

void MapPoint::setLatitude(double latitude) {
    if (qFuzzyCompare(_coordinate.latitude(), latitude)) {
        return;
    }
    _coordinate.setLatitude(latitude);
    emit coordinateChanged();
}

double MapPoint::longitude() const {
    return _coordinate.longitude();
}

void MapPoint::setLongitude(double longitude) {
    if (qFuzzyCompare(_coordinate.longitude(), longitude)) {
        return;
    }
    _coordinate.setLongitude(longitude);
    emit coordinateChanged();
}

QString MapPoint::address() const {
    return _address;
}

void MapPoint::setAddress(const QString& address) {
    if (_address == address) {
        return;
    }
    _address = address;
    emit coordinateChanged();
}