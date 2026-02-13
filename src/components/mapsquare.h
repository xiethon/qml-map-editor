#pragma once

#include "mapgeometry.h"

class MapSquare : public MapGeometry {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(double sideLength READ sideLength WRITE setSideLength NOTIFY sideLengthChanged) //! 边长
    Q_PROPERTY(QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged) //! 中心点

public:
    explicit MapSquare(QGeoCoordinate center, double side_length, QObject* parent = nullptr);
    explicit MapSquare(QGeoCoordinate center, QObject* parent = nullptr);
    explicit MapSquare(QObject* parent = nullptr);
    ~MapSquare() override = default;

    MapSquare(const MapSquare&) = delete;
    MapSquare& operator=(const MapSquare&) = delete;

    double sideLength() const;
    void setSideLength(double _side_length);

    QGeoCoordinate center() const;
    void setCenter(const QGeoCoordinate& center);

signals:
    void sideLengthChanged();
    void centerChanged();

private:
    double _side_length{0.0};
    QGeoCoordinate _center{0.0, 0.0};
};
