#include "P_CanvasWidget.h"

namespace painters {

void p_canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF _a = event->scenePos();
    switch (m_cur_tool) {
    case tool_type::None:
        break;
    case tool_type::Pen:
        MOUSE_EVENT_PRESS(m_cur_brush, p_brush_component);
        break;
    case tool_type::Rect:
        MOUSE_EVENT_PRESS(m_cur_rect, p_rect_component);
        break;
    case tool_type::Circle:
        MOUSE_EVENT_PRESS(m_cur_circle, p_circle_component);
        break;
    case tool_type::Image:
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
