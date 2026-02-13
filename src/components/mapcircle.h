#pragma once

#include "mapgeometry.h"

class MapCircle : public MapGeometry {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged) //! 半径
    Q_PROPERTY(QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged) //! 中心点

public:
    explicit MapCircle(QGeoCoordinate center, double radius, QObject* parent = nullptr);
    explicit MapCircle(QGeoCoordinate center, QObject* parent = nullptr);
    explicit MapCircle(QObject* parent = nullptr);
    ~MapCircle() override = default;

    MapCircle(const MapCircle&) = delete;
    MapCircle& operator=(const MapCircle&) = delete;

    double radius() const;
    void setRadius(double radius);

    QGeoCoordinate center() const;
    void setCenter(const QGeoCoordinate& center);

signals:
    void radiusChanged();
    void centerChanged();

private:
    double _radius{0.0};
    QGeoCoordinate _center{0.0, 0.0};
};