#pragma once

#include "mapgeometry.h"

#include <QList>
#include <QVariantList>
#include <QtPositioning/QGeoPath>

class MapPoint;

class MapPolygon : public MapGeometry {
    Q_OBJECT
    QML_ELEMENT
    Q_MOC_INCLUDE("mappoint.h")

    Q_PROPERTY(QGeoPath path READ path NOTIFY pathChanged) //! 点列表
    Q_PROPERTY(bool valid READ valid NOTIFY pathChanged) //! 是否闭合
    Q_PROPERTY(QVariantList mapPoints READ mapPoints NOTIFY pathChanged) //! 点列表

public:
    explicit MapPolygon(QObject* parent = nullptr);
    ~MapPolygon() override = default;

    MapPolygon(const MapPolygon&) = delete;
    MapPolygon& operator=(const MapPolygon&) = delete;

    QGeoPath path() const;
    void append(QGeoCoordinate coordinate);
    bool valid() const;
    QVariantList mapPoints() const;
    void closeGeometry(); //! 闭合几何对象
    Q_INVOKABLE void setSelectedPoint(const QString& uuid);
    void clearAllPointSelected();
    void setSelected(bool selected) override;

signals:
    void pathChanged();

private:
    QList<MapPoint*> _points{};
};