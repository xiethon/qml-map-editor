#pragma once
//! MapGeometry.h

#include <QObject>
#include <QList>
#include <QPointer>
#include <QVariantList>
#include <QVector>
#include <QtQmlIntegration/QtQmlIntegration>

class MapGeometry : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged) //! 是否选中
    Q_PROPERTY(bool canClose READ canClose NOTIFY canCloseChanged) //! 是否可以闭合
    Q_PROPERTY(bool closed READ closed WRITE setClosed NOTIFY closedChanged) //! 是否已经闭合

public:
    explicit MapGeometry(QObject* parent = nullptr);
    ~MapGeometry() override = default;

    MapGeometry(const MapGeometry&) = delete;
    MapGeometry& operator=(const MapGeometry&) = delete;

    bool selected() const;
    void setSelected(bool selected);
    bool canClose() const;
    bool closed() const;
    void setClosed(bool closed);

protected:
    void setCanCloseCapability(bool can_close);

signals:
    void selectedChanged();
    void canCloseChanged();
    void closedChanged();

private:
    bool _selected{true};
    bool _can_close{false};
    bool _closed{false};
};