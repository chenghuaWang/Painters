#include "P_CanvasWidget.h"

namespace painters {

void p_canvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF _a = event->scenePos();
    switch (m_cur_tool) {
    case tool_type::None:
        break;
    case tool_type::Pen:{
        if (event->button() == Qt::LeftButton) {
            m_cur_brush_enable = true;
            if (_a.x() < 0) _a.setX(0);
            else if (_a.x() > m_scene_size.width()) _a.setX(m_scene_size.width());
            if (_a.y() < 0) _a.setY(0);
            else if (_a.y() > m_scene_size.height()) _a.setY(m_scene_size.height());
            m_cur_brush = new p_brush_component(); //TODO add brush counter.
            m_cur_brush->press_event_from_scene(_a);
            addItem(m_cur_brush);
        }
    }
        break;
    case tool_type::Shape:
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
            if (_a.x() < 0) _a.setX(0);
            else if (_a.x() > m_scene_size.width()) _a.setX(m_scene_size.width());
            if (_a.y() < 0) _a.setY(0);
            else if (_a.y() > m_scene_size.height()) _a.setY(m_scene_size.height());
            m_cur_brush->move_event_from_scene(_a);
        }
    }
        break;
    case tool_type::Shape:
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
            if (_a.x() < 0) _a.setX(0);
            else if (_a.x() > m_scene_size.width()) _a.setX(m_scene_size.width());
            if (_a.y() < 0) _a.setY(0);
            else if (_a.y() > m_scene_size.height()) _a.setY(m_scene_size.height());
            m_cur_brush->release_event_from_scene(_a);
            m_cur_brush = nullptr;
            m_cur_brush_enable = false;
        }
    }
        break;
    case tool_type::Shape:
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
