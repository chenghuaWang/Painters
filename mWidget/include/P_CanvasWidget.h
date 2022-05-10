/*!
 * @author  chenghua.wang
 * @brief   This file for main canvas class.
 *
 * @details Canvas is actually a abstract of Qt Scene.
 * For multi ndoes management. Actually, the layer scene
 * is a concept, no need to use other QtWidget to implement
 * it.
 *
 * I just use a P_layer_manager file to define the node of
 * items in each big item, and each big item will set to scene
 *
 * When users use a select method. It will also check in the
 * node, to dedermine if those selected item is in this scene
 * or not.
 */
#ifndef P_CANVASWIDGET_H
#define P_CANVASWIDGET_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"

#include "P_component.h"
#include <QGraphicsScene>

///< TODO add brush counter.
#define MOUSE_EVENT_PRESS(object_ptr, object_type) \
    if (event->button() == Qt::LeftButton && \
            event->modifiers() != Qt::AltModifier && \
            event->modifiers() != Qt::ShiftModifier) { \
        if (_a.x() < 0 || _a.x() > m_scene_size.width()) break; \
        if (_a.y() < 0 || _a.y() > m_scene_size.height()) break; \
        object_ptr##_enable = true; \
        object_ptr = new object_type(); \
        object_ptr->press_event_from_scene(_a); \
        addItem(object_ptr); \
    } \

#define MOUSE_EVENT_MOVE(object_ptr) \
    if (object_ptr##_enable) { \
        if (_a.x() < 0 || _a.x() > m_scene_size.width()) break; \
        if (_a.y() < 0 || _a.y() > m_scene_size.height()) break; \
        object_ptr->move_event_from_scene(_a); \
    }\

#define MOUSE_EVENT_RELEASE(object_ptr) \
    if (object_ptr##_enable && event->button() == Qt::LeftButton) { \
        object_ptr##_enable = false; \
        if (_a.x() < 0 || _a.x() > m_scene_size.width()) break; \
        if (_a.y() < 0 || _a.y() > m_scene_size.height()) break; \
        object_ptr->release_event_from_scene(_a); \
        object_ptr = nullptr; \
    } \

namespace painters {

    enum class tool_type {
        None = 0,
        Select,
        Pen,
        Rect,
        Circle,
        Image,
        Effect,
        // for pixmap
        Pixmap,
        Pixmap_Pen_mutable,
        Pixmap_Erease_mutable,
    };

    /*!
     * @brief TODO scorrled.
     */
    class p_canvas: public QGraphicsScene {
    public:
        p_canvas() {
            setBackgroundBrush(Qt::white);
            setSceneRect(QRectF(0, 0, m_scene_size.width(), m_scene_size.height()));
            QPen pen;
            pen.setColor(QColor(0, 0, 0));
            pen.setStyle(Qt::PenStyle::DashDotLine);
            pen.setWidth(1);
            QGraphicsRectItem *m_rectItem = new QGraphicsRectItem();
            m_rectItem->setRect(0, 0, 1024, 720);
            m_rectItem->setPen(pen);
            m_rectItem->setBrush(QBrush(QColor(255, 255, 255)));
            addItem(m_rectItem);
        }

    protected: ///< overide original virtual functions.
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    public: ///< Interface to other class.


    private: ///< numerous flags setting.
        tool_type           m_cur_tool = tool_type::Circle;
        bool                m_cur_brush_enable = false;
        bool                m_cur_rect_enable = false;
        bool                m_cur_circle_enable = false;

    private: ///< scene global data
        QSize               m_scene_size = QSize(1024, 720);

    private: ///< temporary data
        p_brush_component   *m_cur_brush = nullptr;
        p_rect_component    *m_cur_rect = nullptr;
        p_circle_component  *m_cur_circle =  nullptr;
    };

}

#endif // P_CANVASWIDGET_H
