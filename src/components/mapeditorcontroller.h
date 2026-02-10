#pragma once

#include <QtCore/QObject>
#include <QVariantList>
#include <QtPositioning/QGeoCoordinate>
#include <QtQmlIntegration/QtQmlIntegration>

class MapPoint;
class MapLineString;
class MapPolygon;

class MapEditorController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_MOC_INCLUDE("mappoint.h")
    Q_MOC_INCLUDE("maplinestring.h")
    Q_MOC_INCLUDE("mappolygon.h")

    Q_PROPERTY(EditMode editMode READ editMode WRITE setEditMode NOTIFY editModeChanged)
    Q_PROPERTY(bool locked READ locked WRITE setLocked NOTIFY lockedChanged)
    Q_PROPERTY(bool canClose READ canClose NOTIFY canCloseChanged)

    Q_PROPERTY(QVariantList mapPoints READ mapPoints NOTIFY mapPointsChanged)
    Q_PROPERTY(QVariantList mapPolygons READ mapPolygons NOTIFY mapPolygonsChanged)
    // Q_PROPERTY(QVariantList mapLineStrings READ mapLineStrings NOTIFY mapLineStringsChanged)

public:
    enum EditMode { POINT = 0, LINESTRING, POLYGON, CIRCLE, SQUARE }; //! 编辑模式
    Q_ENUM(EditMode)

    explicit MapEditorController(QObject* parent = nullptr);

    EditMode editMode() const; //! 获取当前编辑模式
    void setEditMode(EditMode mode); //! 设置当前编辑模式

    QVariantList mapPoints() const; //! 获取所有标记点
    QVariantList mapPolygons() const; //! 获取所有多边形

    // QVariantList mapLineStrings() const; //! 获取所有线

    bool locked() const; //! 获取编辑锁状态
    void setLocked(bool locked); //! 设置编辑锁状态
    bool canClose() const; //! 获取当前编辑对象是否可以闭合

    Q_INVOKABLE void append(QGeoCoordinate coordinate); //! 添加
    Q_INVOKABLE void clearAll(); //! 删除所有数据
    Q_INVOKABLE void deleteSelected(); //! 删除选中数据
    Q_INVOKABLE void closeCurrentGeometry(); //! 闭合当前编辑对象
    Q_INVOKABLE void setSelectedItemAndClearOthers(int item_id); //! 设置选中项，并清除其他项的选中状态
    Q_INVOKABLE void setSelectedItem(int item_id); //! 设置选中项，不清除其他项的选中状态
    Q_INVOKABLE void clearAllSelected(); //! 清除所有选中状态

private:
    void clearAllSelectedForMode(EditMode mode); //! 清除当前编辑模式下所有选中状态

signals:
    void editModeChanged(); //! 编辑模式改变信号
    void lockedChanged(); //! 编辑锁状态改变信号
    void canCloseChanged(); //! 是否可以闭合改变信号

    void mapPointsChanged(); //! 标记点列表改变信号
    void mapLineStringsChanged(); //! 线列表改变信号
    void mapPolygonsChanged(); //! 多边形列表改变信号

private:
    EditMode _edit_mode{EditMode::POINT}; //! 当前编辑模式
    bool _locked{false}; //! 编辑锁，防止在编辑过程中进行其他操作

    QList<MapPoint*> _map_points{}; //! MapPoint列表
    QList<MapLineString*> _map_linestrings{}; //! MapLineString列表
    QList<MapPolygon*> _map_polygons{}; //! MapPolygon列表
};
