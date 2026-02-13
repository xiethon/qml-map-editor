#pragma once

#include <QObject>
#include <QList>
#include <QPointer>
#include <QVariantList>
#include <QVector>
#include <QUuid>
#include <QtQmlIntegration/QtQmlIntegration>
#include <QtPositioning/QGeoCoordinate>

class MapGeometry : public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    enum class GeometryType { Unknown, Point, LineString, Polygon, Circle, Square };
    Q_ENUM(GeometryType)

    Q_PROPERTY(QString uuid READ uuid CONSTANT) //! 唯一标识符
    Q_PROPERTY(GeometryType geometryType READ geometryType CONSTANT) //! 几何类型
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged) //! 是否选中
    Q_PROPERTY(bool closed READ closed NOTIFY closedChanged) //! 是否已经闭合
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged) //! 是否有效
    Q_PROPERTY(bool hasChildren READ hasChildren NOTIFY hasChildrenChanged) //! 是否有子项

public:
    explicit MapGeometry(GeometryType type = GeometryType::Unknown, QObject* parent = nullptr);
    static QString geometryTypeToString(GeometryType type);

    static double distanceToSegment(
        const QGeoCoordinate& point, const QGeoCoordinate& segment_start, const QGeoCoordinate& segment_end);

    ~MapGeometry() override = default;

    MapGeometry(const MapGeometry&) = delete;
    MapGeometry& operator=(const MapGeometry&) = delete;

    QString uuid() const;

    //! 几何类型
    GeometryType geometryType() const;

    //! 选中状态
    virtual void setSelected(bool selected);
    virtual bool selected() const;

    //! 闭合状态
    virtual bool closed() const;
    virtual void setClosed(bool closed);

    //! 有效状态
    virtual bool valid() const;
    virtual void setValid(bool valid);

    //! 是否有子项
    virtual bool hasChildren() const;
    virtual QList<MapGeometry*> children() const;
    virtual void setSelectedChild(QString uuid);
    virtual void setAllChildrenSelected(bool selected);

    //! 删除选中子项
    virtual void removeSelectedChild();
    virtual void appendChild(QGeoCoordinate coordinate);

signals:
    void selectedChanged();
    void closedChanged();
    void validChanged();
    void hasChildrenChanged();

protected:
    void setHasChildren(bool has_children);

private:
    const QString _uuid{QUuid::createUuid().toString(QUuid::WithoutBraces)};
    const GeometryType _geometry_type{GeometryType::Unknown};
    bool _selected{true};
    bool _closed{false};
    bool _valid{true};
    bool _has_children{false};
};