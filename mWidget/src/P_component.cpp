#include "P_component.h"
#include <QPainter>
#include <QRect>

//if(isSelected()){\
//    QRectF bound_rect = boundingRect();\
//    QRectF _out_line_ = bound_rect.adjusted(-m_payload.m_interval,\
//                                            -m_payload.m_interval,\
//                                            m_payload.m_interval,\
//                                            m_payload.m_interval);\
//    QPointF pos = event->pos();\
//    QPointF scene_pos = event->scenePos();\
//    if (m_payload.cauculate_distance(pos, _out_line_.bottomRight()) <= m_payload.m_active_area_r){\
//        m_payload.m_op_type = p_op_type::Resize;\
//        qDebug() << "resize";\
//    }\
//    else if (m_payload.cauculate_distance(pos, _out_line_.bottomLeft()) <= m_payload.m_active_area_r){\
//        m_payload.m_op_type = p_op_type::Rotate;\
//        qDebug() << "rotate";\
//    }\
//}

namespace painters {
QImage    p_op_payload::m_close_icon;
QImage    p_op_payload::m_resize_icon;
QImage    p_op_payload::m_rotate_icon;
}

namespace painters {

OVERLOAD_FUNC_IMPL(p_brush_component, QGraphicsPathItem);

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

    if (m_interpolation_enable) {
        _interpolation_(0);
    }

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

void p_brush_component::set_movable(bool enable) {
    if (enable) {
        setFlag(QGraphicsItem::ItemIsMovable);
    } else {

    }
}
}; //! namespace painters

namespace painters {

OVERLOAD_FUNC_IMPL(p_rect_component, QGraphicsRectItem);

void p_rect_component::__init__() {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void p_rect_component::move_event_from_scene(const QPointF &_a) {
    m_end_point = _a;

    auto [v1, v2] = __get_rect_payload__();
    setRect(v1.x(), v1.y(), v2.x(), v2.y());

    this->update();
}

void p_rect_component::press_event_from_scene(const QPointF &_a) {
    m_start_point = _a;
    m_end_point = _a;
}

void p_rect_component::release_event_from_scene(const QPointF &_a) {
    m_end_point = _a;
    m_center_point = (m_start_point + m_end_point) / 2;

    auto [v1, v2] = __get_rect_payload__();
    setRect(v1.x(), v1.y(), v2.x(), v2.y());

    // this->setPos(m_center_point);
    this->prepareGeometryChange();
    this->update();
    m_dirty = true;
}

std::pair<QPointF, QPointF> p_rect_component::__get_rect_payload__() {
    double x1 = m_start_point.x();
    double y1 = m_start_point.y();
    double x2 = m_end_point.x();
    double y2 = m_end_point.y();

    double left_top_x = P_MIN(x1, x2);
    double left_top_y = P_MIN(y1, y2);
    double right_bottom_x = P_MAX(x1, x2);
    double right_bottom_y = P_MAX(y1, y2);

    double w = right_bottom_x - left_top_x;
    double h = right_bottom_y - left_top_y;

    return std::pair<QPointF, QPointF>({left_top_x, left_top_y}, {w, h});
}

} //! namespace painters

namespace painters {

    OVERLOAD_FUNC_IMPL(p_circle_component, QGraphicsEllipseItem);

    void p_circle_component::__init__() {
        setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    }

    void p_circle_component::move_event_from_scene(const QPointF &_a) {
        m_end_point = _a;

        auto [v1, v2] = __get_rect_payload__();
        setRect(v1.x(), v1.y(), v2.x(), v2.y());

        this->update();
    }

    void p_circle_component::press_event_from_scene(const QPointF &_a) {
        m_start_point= _a;
        m_end_point = _a;
    }

    void p_circle_component::release_event_from_scene(const QPointF &_a) {
        m_end_point = _a;
        m_center_point = (m_start_point + m_end_point) / 2;

        auto [v1, v2] = __get_rect_payload__();
        setRect(v1.x(), v1.y(), v2.x(), v2.y());

        //this->setPos(m_center_point);
        this->prepareGeometryChange();
        this->update();
        m_dirty = true;
    }

    std::pair<QPointF, QPointF> p_circle_component::__get_rect_payload__() {
        double x1 = m_start_point.x();
        double y1 = m_start_point.y();
        double x2 = m_end_point.x();
        double y2 = m_end_point.y();

        double left_top_x = P_MIN(x1, x2);
        double left_top_y = P_MIN(y1, y2);
        double right_bottom_x = P_MAX(x1, x2);
        double right_bottom_y = P_MAX(y1, y2);

        double w = right_bottom_x - left_top_x;
        double h = right_bottom_y - left_top_y;

        return std::pair<QPointF, QPointF>({left_top_x, left_top_y}, {w, h});
    }
}

namespace painters {

OVERLOAD_FUNC_IMPL(p_image_component, QGraphicsPixmapItem);

void p_image_component::__init__() {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void p_image_component::move_event_from_scene(const QPointF &_a) {
    this->update();
}

void p_image_component::press_event_from_scene(const QPointF &_a) {
    return;
}

void p_image_component::release_event_from_scene(const QPointF &_a) {
    this->prepareGeometryChange();
    this->update();
}

}
