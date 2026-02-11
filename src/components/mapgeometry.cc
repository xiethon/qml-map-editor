#include "mapgeometry.h"

MapGeometry::MapGeometry(QObject* parent) : QObject(parent) {}

QString MapGeometry::uuid() const {
    return _uuid;
}

bool MapGeometry::selected() const {
    return _selected;
}

void MapGeometry::setSelected(bool selected) {
    if (_selected == selected) {
        return;
    }
    _selected = selected;
    emit selectedChanged();
}

bool MapGeometry::closed() const {
    return _closed;
}

void MapGeometry::setClosed(bool closed) {
    if (_closed == closed) {
        return;
    }
    _closed = closed;
    emit closedChanged();
}
