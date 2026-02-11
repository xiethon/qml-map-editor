#pragma once
//! MapGeometry.h

#include <QObject>
#include <QList>
#include <QPointer>
#include <QVariantList>
#include <QVector>
#include <QUuid>
#include <QtQmlIntegration/QtQmlIntegration>

class MapGeometry : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString uuid READ uuid CONSTANT) //! 唯一标识符
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged) //! 是否选中
    Q_PROPERTY(bool closed READ closed WRITE setClosed NOTIFY closedChanged) //! 是否已经闭合

public:
    explicit MapGeometry(QObject* parent = nullptr);
    ~MapGeometry() override = default;

    MapGeometry(const MapGeometry&) = delete;
    MapGeometry& operator=(const MapGeometry&) = delete;

    QString uuid() const;

    virtual void setSelected(bool selected);
    bool selected() const;
    bool closed() const;
    void setClosed(bool closed);

signals:
    void selectedChanged();
    void closedChanged();

private:
    const QString _uuid{QUuid::createUuid().toString(QUuid::WithoutBraces)};
    bool _selected{true};
    bool _closed{false};
};