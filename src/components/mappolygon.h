#pragma once
//! MapPolygon.h

#include "mapgeometry.h"
#include "mappoint.h"
#include <QList>
#include <QVariantList>

class MapPolygon : public MapGeometry {
    Q_OBJECT
    QML_ELEMENT
    Q_MOC_INCLUDE("mappoint.h")

    Q_PROPERTY(QVariantList path READ path NOTIFY pathChanged) //! 点列表

public:
    explicit MapPolygon(QObject* parent = nullptr);
    ~MapPolygon() override = default;

    MapPolygon(const MapPolygon&) = delete;
    MapPolygon& operator=(const MapPolygon&) = delete;

    QVariantList path() const;
    void append(QGeoCoordinate coordinate);

signals:
    void pathChanged();

private:
    QList<MapPoint*> _points{};
};