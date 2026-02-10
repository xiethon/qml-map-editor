#include "mapgeometry.h"

MapGeometry::MapGeometry(QObject* parent) : QObject(parent) {}

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
    if (!_can_close && closed) {
        return;
    }
    if (_closed == closed) {
        return;
    }
    _closed = closed;
    emit closedChanged();
}

bool MapGeometry::canClose() const {
    return _can_close;
}

void MapGeometry::setCanCloseCapability(bool can_close) {
    if (_can_close == can_close) {
        return;
    }
    _can_close = can_close;
    emit canCloseChanged();
    if (!_can_close && _closed) {
        _closed = false;
        emit closedChanged();
    }
}
