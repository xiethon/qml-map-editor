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
    Q_PROPERTY(QVariantList mapPoints READ mapPoints NOTIFY pathChanged) //! 点列表

public:
    explicit MapLineString(QObject* parent = nullptr);
    ~MapLineString() override = default;

    MapLineString(const MapLineString&) = delete;
    MapLineString& operator=(const MapLineString&) = delete;

    QGeoPath path() const;
    QVariantList mapPoints() const;

    bool valid() const override;

    QList<MapGeometry*> children() const override; //! 获取children
    void removeSelectedChild() override; //! 删除选中子项
    void appendChild(QGeoCoordinate coordinate) override; //! 在子项中添加一个点

signals:
    void pathChanged();

private:
    QList<MapPoint*> _points;
};