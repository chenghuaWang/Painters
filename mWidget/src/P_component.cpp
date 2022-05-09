#include "P_component.h"

namespace painters {

void p_brush_component::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

}

void p_brush_component::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::AltModifier) {
            setSelected(true);
        }
        else if (event->modifiers() == Qt::ShiftModifier) {

        }
    }
    else {
        QGraphicsItem::mousePressEvent(event);
        event->ignore();
    }
}
void p_brush_component::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
}

void p_brush_component::move_event_from_scene(const QPointF &_a) {
    m_path.lineTo(_a);
    m_points.append(_a);
    setPath(m_path);
    this->update(); // redraw
}

void p_brush_component::press_event_from_scene(const QPointF &_a) {
    m_start_point = _a;
    m_path.moveTo(m_start_point);
    QRectF bound_rect = m_path.boundingRect();
    m_bounding_box_size = QSize(bound_rect.width(), bound_rect.height());
    setPath(m_path);
    this->update(); // redraw.
}

void p_brush_component::release_event_from_scene(const QPointF &_a) {
    m_end_point = _a;
    m_path.lineTo(_a);
    m_points.append(_a);

    if (m_points.size() == 1) {
        auto &tmp = m_points[0];
        QPointF point_due(tmp.x()+1, tmp.y()+1);
        m_path.lineTo(point_due);
        m_points.append(point_due);
    }

    _interpolation_(0);

    QRectF bound_rect = m_path.boundingRect();
    m_bounding_box_size = QSize(bound_rect.width(), bound_rect.height());
    QPointF center_pos = bound_rect.center();
    this->setPos(center_pos);
    m_path.translate(-center_pos.x(), -center_pos.y());
    this->prepareGeometryChange();
    setPath(m_path);
    this->update(); // redraw
    m_dirty = true;
}

void p_brush_component::_interpolation_(uint32_t samples) {
    m_path.clear(); ///< clear all first.
    QVector<QPointF> tmp_points;
    size_t points_len = m_points.size();
    for (size_t i = 0; i < points_len - 1; ++i ){
        tmp_points.push_back(m_points[i]);
        float x1 = m_points[i].x();
        float y1 = m_points[i].y();
        float x2 = m_points[i + 1].x();
        float y2 = m_points[i + 1].y();
        tmp_points.push_back({(x1+x2)/2, (y1+y2)/2});
    }
    tmp_points.push_back(m_points[points_len-1]);

    m_path.moveTo(tmp_points[0]);
    size_t tmp_points_len = tmp_points.size();

    if (tmp_points_len == 1) return;
    for (size_t i = 1; i < tmp_points_len; ++i) {
        m_path.lineTo(tmp_points[i]);
    }
    setPath(m_path);
    this->update();

    m_points = tmp_points;
}

void p_brush_component::__init__() {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void p_brush_component::set_selectable(bool enable) {
    if (enable) {
        setFlag(QGraphicsItem::ItemIsSelectable);
    } else {
        // TODO
    }
}

void set_movable(bool enable);

}; //! namespace painters
