#include "P_CanvasWidget.h"

namespace painters {

void p_canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF _a = event->scenePos();
    switch (m_cur_tool) {
    case tool_type::None:
        break;
    case tool_type::Pen:{
        if (event->button() == Qt::LeftButton &&
                event->modifiers() != Qt::AltModifier &&
                event->modifiers() != Qt::ShiftModifier) {
            if (_a.x() < 0 || _a.x() > m_scene_size.width()) break;
            if (_a.y() < 0 || _a.y() > m_scene_size.height()) break;
            m_cur_brush_enable = true;
            m_cur_brush = new p_brush_component(); //TODO add brush counter.
            m_cur_brush->press_event_from_scene(_a);
            addItem(m_cur_brush);
        }
    }
        break;
    case tool_type::Rect:
        if (event->button() == Qt::LeftButton &&
                event->modifiers() != Qt::AltModifier &&
                event->modifiers() != Qt::ShiftModifier) {
            if (_a.x() < 0 || _a.x() > m_scene_size.width()) break;
            if (_a.y() < 0 || _a.y() > m_scene_size.height()) break;
            m_cur_rect_enable = true;
            m_cur_rect = new p_rect_component(); //TODO add Rect counter.
            m_cur_rect->press_event_from_scene(_a);
            addItem(m_cur_brush);
        }
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
    case tool_type::Pen:{
        if (m_cur_brush_enable) {
            if (_a.x() < 0 || _a.x() > m_scene_size.width()) break;
            if (_a.y() < 0 || _a.y() > m_scene_size.height()) break;

            m_cur_brush->move_event_from_scene(_a);
        }
    }
        break;
    case tool_type::Rect:
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
    case tool_type::Pen:{
        if (m_cur_brush_enable && event->button() == Qt::LeftButton) {
            m_cur_brush_enable = false;
            if (_a.x() < 0 || _a.x() > m_scene_size.width()) break;
            if (_a.y() < 0 || _a.y() > m_scene_size.height()) break;

            m_cur_brush->release_event_from_scene(_a);
            m_cur_brush = nullptr;
        }
    }
        break;
    case tool_type::Rect:
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
