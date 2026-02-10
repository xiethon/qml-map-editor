#pragma once
//! MapLineString.h

#include "mappoint.h"
#include <QList>
#include <QVariantList>
#include <QtPositioning/QGeoPath>

class MapPoint;

class MapLineString : public MapGeometry {
    Q_OBJECT
    QML_ELEMENT
    Q_MOC_INCLUDE("mappoint.h")

    Q_PROPERTY(QGeoPath path READ path NOTIFY pathChanged) //! 路径
    Q_PROPERTY(QVariantList points READ points NOTIFY pathChanged) //! 点数量
    Q_PROPERTY(int size READ size NOTIFY sizeChanged) //! 点数量

public:
    explicit MapLineString(QObject* parent = nullptr);
    ~MapLineString() override = default;

    MapLineString(const MapLineString&) = delete;
    MapLineString& operator=(const MapLineString&) = delete;

    QGeoPath path() const;
    QVariantList points() const;

    int size() const;
    void append(QGeoCoordinate coordinate);

signals:
    void pathChanged();
    void sizeChanged();

private:
    QList<MapPoint*> _points;
};