#pragma once
//! MapPoint.h

#include "mapgeometry.h"
#include <QtPositioning/QGeoCoordinate>

class MapPoint : public MapGeometry {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged) //! 坐标
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY coordinateChanged) //! 纬度
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY coordinateChanged) //! 经度
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY coordinateChanged) //! 地址

public:
    explicit MapPoint(QObject* parent = nullptr);
    explicit MapPoint(const QGeoCoordinate& coordinate, QObject* parent = nullptr);

    ~MapPoint() override = default;

    MapPoint(const MapPoint&) = delete;
    MapPoint& operator=(const MapPoint&) = delete;
    bool operator==(const MapPoint& other) const;

    QGeoCoordinate coordinate() const;
    void setCoordinate(const QGeoCoordinate& coordinate);

    double latitude() const;
    void setLatitude(double latitude);

    double longitude() const;
    void setLongitude(double longitude);
    QString address() const;
    void setAddress(const QString& address);

signals:
    void coordinateChanged();

private:
    QGeoCoordinate _coordinate{0.0, 0.0};
    QString _address{""};
};