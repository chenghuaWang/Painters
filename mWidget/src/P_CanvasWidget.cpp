#include "P_CanvasWidget.h"

namespace painters {

void p_canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF _a = event->scenePos();
    switch (m_cur_tool) {
    case tool_type::None:
        break;
    case tool_type::Pen:
        MOUSE_EVENT_PRESS(m_cur_brush, p_brush_component, m_cur_brush->set_pen(m_cur_brush_pen));
        break;
    case tool_type::Rect:
        MOUSE_EVENT_PRESS(m_cur_rect, p_rect_component, ;);
        break;
    case tool_type::Circle:
        MOUSE_EVENT_PRESS(m_cur_circle, p_circle_component, ;);
        break;
    case tool_type::Image:
        if (event->button() == Qt::LeftButton &&
                event->modifiers() != Qt::AltModifier &&
                event->modifiers() != Qt::ShiftModifier) {
            if (_a.x() < 0 || _a.x() > m_scene_size.width()) break;
            if (_a.y() < 0 || _a.y() > m_scene_size.height()) break;
            m_cur_image_enable = true;
            m_cur_image_string = "D:/Imgs/ke.jpg";
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

    // broadcast this event to other component.
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

    // broadcast this event to other component.
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

    // broadcast this event to other component.
    QGraphicsScene::mouseMoveEvent(event);
}

} //! namespace painters

namespace painters {
void p_canvas::slots_brush_pen_changed(QPen &_a){
    m_cur_brush_pen = _a;
}
}
