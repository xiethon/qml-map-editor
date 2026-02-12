#include "mapgeometry.h"

MapGeometry::MapGeometry(GeometryType type, QObject* parent) : QObject(parent), _geometry_type(type) {}

MapGeometry::GeometryType MapGeometry::geometryType() const {
    return _geometry_type;
}

QString MapGeometry::geometryTypeToString(GeometryType type) {
    switch (type) {
        case GeometryType::Point:
            return "Point";
        case GeometryType::LineString:
            return "LineString";
        case GeometryType::Polygon:
            return "Polygon";
        default:
            return "Unknown";
    }
}

double MapGeometry::distanceToSegment(
    const QGeoCoordinate& point, const QGeoCoordinate& segment_start, const QGeoCoordinate& segment_end) {
    double x0 = point.longitude();
    double y0 = point.latitude();
    double x1 = segment_start.longitude();
    double y1 = segment_start.latitude();
    double x2 = segment_end.longitude();
    double y2 = segment_end.latitude();

    double dx = x2 - x1;
    double dy = y2 - y1;

    if (dx == 0 && dy == 0) {
        dx = x0 - x1;
        dy = y0 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }
    double t = ((x0 - x1) * dx + (y0 - y1) * dy) / (dx * dx + dy * dy);
    t = std::max(0.0, std::min(1.0, t));
    double projX = x1 + t * dx;
    double projY = y1 + t * dy;
    dx = x0 - projX;
    dy = y0 - projY;
    return std::sqrt(dx * dx + dy * dy);
}

QString MapGeometry::uuid() const {
    return _uuid;
}

bool MapGeometry::selected() const {
    return _selected;
}

void MapGeometry::setSelected(bool selected) {
    setAllChildrenSelected(false);
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

bool MapGeometry::valid() const {
    return _valid;
}

void MapGeometry::setValid(bool valid) {
    if (_valid == valid) {
        return;
    }
    _valid = valid;
    emit validChanged();
}

bool MapGeometry::hasChildren() const {
    return _has_children;
}

QList<MapGeometry*> MapGeometry::children() const {
    return QList<MapGeometry*>{};
}

void MapGeometry::setSelectedChild(QString uuid) {
    auto children = this->children();
    for (auto& child : children) {
        if (child->uuid() == uuid) {
            child->setSelected(true);
        } else {
            child->setSelected(false);
        }
    }
}

void MapGeometry::setAllChildrenSelected(bool selected) {
    auto children = this->children();
    for (auto& child : children) {
        child->setSelected(selected);
    }
}

void MapGeometry::setHasChildren(bool has_children) {
    if (_has_children == has_children) {
        return;
    }
    _has_children = has_children;
    emit hasChildrenChanged();
}

void MapGeometry::removeSelectedChild() {}

void MapGeometry::appendChild(QGeoCoordinate coordinate) {
    Q_UNUSED(coordinate);
}