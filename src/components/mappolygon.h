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
    Q_PROPERTY(QVariantList mapPoints READ mapPoints NOTIFY pathChanged) //! 点列表

public:
    explicit MapPolygon(QObject* parent = nullptr);
    ~MapPolygon() override = default;

    MapPolygon(const MapPolygon&) = delete;
    MapPolygon& operator=(const MapPolygon&) = delete;

    QGeoPath path() const; //! geopath路径

    QVariantList mapPoints() const; //! 获取所有点

    bool valid() const override; //! 是否有效的多边形
    QList<MapGeometry*> children() const override; //! 获取children

    void removeSelectedChild() override; //! 删除选中子项
    void appendChild(QGeoCoordinate coordinate) override; //! 在子项中添加一个点

signals:
    void pathChanged();

private:
    QList<MapPoint*> _points{};
};