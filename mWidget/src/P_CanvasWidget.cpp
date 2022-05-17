#include "P_CanvasWidget.h"

namespace painters {

p_canvas *p_canvas::m_s_instance = nullptr;

p_canvas* p_canvas::get_instance() {
    return m_s_instance;
}

void p_canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF _a = event->scenePos();
    switch (m_cur_tool) {
    case tool_type::None:
        break;
    case tool_type::Pen:
        MOUSE_EVENT_PRESS(m_cur_brush, p_brush_component, m_cur_brush->set_pen(m_cur_brush_pen));
        break;
    case tool_type::Rect:
        MOUSE_EVENT_PRESS(m_cur_rect, p_rect_component, (m_cur_rect->set_brush(m_cur_rect_brush), m_cur_rect->set_pen(m_cur_rect_pen)));
        break;
    case tool_type::Circle:
        MOUSE_EVENT_PRESS(m_cur_circle, p_circle_component, (m_cur_circle->set_brush(m_cur_circle_brush), m_cur_circle->set_pen(m_cur_circle_pen)));
        break;
    case tool_type::Image:
        if (event->button() == Qt::LeftButton &&
                event->modifiers() != Qt::AltModifier &&
                event->modifiers() != Qt::ShiftModifier) {
            if (_a.x() < 0 || _a.x() > m_scene_size.width()) break;
            if (_a.y() < 0 || _a.y() > m_scene_size.height()) break;
            m_cur_image_enable = true;
//            m_cur_image_string = "D:/Imgs/ke.jpg";
            m_cur_image = new p_image_component(m_cur_image_string, __combine_name__(m_cur_image_cnt));
            m_cur_image->press_event_from_scene(_a);
            addItem(m_cur_image);
            m_cur_choosed_layer->add_node(m_cur_image->get_name(), m_cur_image);
        }
        break;
    case tool_type::Select:
        break;
    case tool_type::Effect:
        break;
    default:
        break;
    }

    if (m_cur_tool == tool_type::Select) {
        if (m_choosed_for_select == nullptr) {
            QTransform transform;
            m_choosed_for_select = this->itemAt(_a, transform);
            m_choosed_for_select->setSelected(true);
        }
        else {
            QRectF bound_rect = m_choosed_for_select->boundingRect();
            QRectF _out_line_ = bound_rect.adjusted(-4, -4, 4, 4);
            QPointF pos = event->pos();
            QPointF scene_pos = event->scenePos();

            if (cauculate_distance(scene_pos, m_choosed_for_select->mapToScene(_out_line_.topRight())) <= 16.0){
                m_op_type = p_op_type::None;
                m_cur_choosed_layer->delete_node(m_choosed_for_select);

                this->removeItem(m_choosed_for_select);
                m_choosed_for_select = nullptr;
                this->update();

                emit signal_update_layer_tree();
            }
            else if (cauculate_distance(scene_pos, m_choosed_for_select->mapToScene(_out_line_.bottomRight())) <= 16.0) {

            }
            else if (cauculate_distance(scene_pos, m_choosed_for_select->mapToScene(_out_line_.bottomLeft())) <= 16.0) {

            } else {
                m_choosed_for_select->setSelected(false);
                QTransform transform;
                m_choosed_for_select = this->itemAt(_a, transform);
                m_choosed_for_select->setSelected(true);
            }
        }
    }
    // broadcast this event to other component. TODO
    QGraphicsScene::mousePressEvent(event);
}

void p_canvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF _a = event->scenePos();
    switch (m_cur_tool) {
    case tool_type::None:
        break;
    case tool_type::Pen:
        MOUSE_EVENT_MOVE(m_cur_brush);
        break;
    case tool_type::Rect:
        MOUSE_EVENT_MOVE(m_cur_rect);
        break;
    case tool_type::Circle:
        MOUSE_EVENT_MOVE(m_cur_circle);
        break;
    case tool_type::Image:
        MOUSE_EVENT_MOVE(m_cur_image);
        break;
    case tool_type::Select:
        break;
    case tool_type::Effect:
        break;
    default:
        break;
    }

    // broadcast this event to other component. TODO
    QGraphicsScene::mouseMoveEvent(event);
}

void p_canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QPointF _a = event->scenePos();
    switch (m_cur_tool) {
    case tool_type::None:
        break;
    case tool_type::Pen:
        MOUSE_EVENT_RELEASE(m_cur_brush);
        break;
    case tool_type::Rect:
        MOUSE_EVENT_RELEASE(m_cur_rect);
        break;
    case tool_type::Circle:
        MOUSE_EVENT_RELEASE(m_cur_circle);
        break;
    case tool_type::Image:
        MOUSE_EVENT_RELEASE(m_cur_image);
        break;
    case tool_type::Select:
        break;
    case tool_type::Effect:
        break;
    default:
        break;
    }

    // broadcast this event to other component. TODO
    QGraphicsScene::mouseReleaseEvent(event);
}

} //! namespace painters

namespace painters {

void p_canvas::set_cur_choosed_layer(const REF(p_graphic_layer)& _a) {
    m_cur_choosed_layer = _a;
}

void p_canvas::slots_brush_pen_changed(QPen &_a){
    m_cur_brush_pen = _a;
}

void p_canvas::slots_draw_reference_line() {
    int h = (int)height();
    int w = (int)width();

    QPainterPath tmp;
    QPainterPath tmp2;

    tmp.moveTo(0, 0);
    int t = 160;
    for (int i = 0; i < w; i+=t) {
        if ((i) < w) tmp.lineTo(i , 0);
        if ((i + 40) < w) tmp.lineTo(i + 40, 0);
        if ((i + 40) < w) tmp.lineTo(i + 40, h - 1);
        if ((i + 80) < w) tmp.lineTo(i + 80, h - 1);
        if ((i + 80) < w) tmp.lineTo(i + 80, 0);
        if ((i + 120) < w) tmp.lineTo(i + 120, 0);
        if ((i + 120) < w) tmp.lineTo(i + 120, h - 1);
        if ((i + 160) < w) tmp.lineTo(i + 160, h - 1);
    }

    tmp2.moveTo(0, 0);
    for (int i = 0; i < h; i+=t) {
        if ((i) < w) tmp2.lineTo(0 , i);
        if ((i + 40) < h) tmp2.lineTo(0, i + 40);
        if ((i + 40) < h) tmp2.lineTo(w - 1, i + 40);
        if ((i + 80) < h) tmp2.lineTo(w - 1, i + 80);
        if ((i + 80) < h) tmp2.lineTo(0 , i + 80);
        if ((i + 120) < h) tmp2.lineTo(0, i + 120);
        if ((i + 120) < h) tmp2.lineTo(w - 1, i + 120);
        if ((i + 160) < h) tmp2.lineTo(w - 1, i + 160);
    }

    QPen qpen;
    qpen.setColor(QColor(0, 0, 0, 64));
    qpen.setStyle(Qt::PenStyle::DashLine);
    qpen.setWidth(1);

    m_reference_line = new QGraphicsPathItem();
    m_reference_line->setPath(tmp);
    m_reference_line->setPen(qpen);
//    addItem(m_reference_line);

    m_reference_line_v = new QGraphicsPathItem();
    m_reference_line_v->setPath(tmp2);
    m_reference_line_v->setPen(qpen);
//    addItem(m_reference_line_v);
    QBrush m_brush;
    m_brush.setColor(QColor(0, 0, 0, 64));
    m_brush.setStyle(Qt::BrushStyle::CrossPattern);

    m_rectItem->setBrush(m_brush);
    this->update();
}

void p_canvas::slots_delete_refernce_line() {
    if (m_reference_line != nullptr) {
//        removeItem(m_reference_line);
//        removeItem(m_reference_line_v);
        delete m_reference_line;
        delete m_reference_line_v;
        m_reference_line = nullptr;
        m_reference_line_v = nullptr;
    }
    QBrush m_brush;
    m_brush.setColor(QColor(0, 0, 0, 64));
    m_brush.setStyle(Qt::BrushStyle::NoBrush);

    m_rectItem->setBrush(m_brush);
    this->update();
}

void p_canvas::slots_process_reference(const QString& _a) {
    if (_a == "None") {
        slots_delete_refernce_line();
    }
    else if (_a == "default") {
        slots_draw_reference_line();
    }
}

void p_canvas::slots_lock_cur_layer(bool enable) {
    m_cur_choosed_layer->m_locked = enable;
}

void p_canvas::slots_set_tool_type_shape(bool enable) {
    m_cur_tool = tool_type::Rect;
}

void p_canvas::slots_set_tool_type_pen(bool enable) {
    m_cur_tool = tool_type::Pen;
}

void p_canvas::slots_set_tool_type_image(bool enable) {
    m_cur_tool = tool_type::Image;
}

void p_canvas::slots_shape_brush_changed(QBrush &_a) {
    m_cur_rect_brush = _a;
    m_cur_circle_brush = _a;
}

void p_canvas::slots_shape_pen_changed(QPen &_a) {
    m_cur_rect_pen = _a;
    m_cur_circle_pen = _a;
}

void p_canvas::slots_image_string_changed(const QString &_a) {
    m_cur_image_string = _a.toStdString();
}

}
