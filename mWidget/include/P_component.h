/*!
 * @author  chenghua.wang
 * @brief   All Graphic item is abstract to component.
 */

#ifndef P_COMPONENT_H
#define P_COMPONENT_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#define OVERLOAD_EVENT protected
#define OVERLOAD_FUNC \
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override; \
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; \
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; \

#define OVERLOAD_FUNC_IMPL(name) \
    void name::mouseMoveEvent(QGraphicsSceneMouseEvent *event) { \
        if (event->modifiers() == Qt::AltModifier && isSelected()) { \
            QGraphicsItem::mouseMoveEvent(event); \
        } \
    } \
    void name::mousePressEvent(QGraphicsSceneMouseEvent *event) {\
        if (event->button() == Qt::LeftButton) {\
            if (event->modifiers() == Qt::AltModifier) {\
                setSelected(true);\
            }\
            else if (event->modifiers() == Qt::ShiftModifier) {\
            } \
        } \
        else { \
            event->ignore(); \
        } \
    } \
    void name::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {\
        QGraphicsItem::mouseReleaseEvent(event); \
    }

#define EVENT_FROM_SCENE public
#define EVENT_FROM_SCENE_FUNC \
    void move_event_from_scene(const QPointF &_a); \
    void press_event_from_scene(const QPointF &_a); \
    void release_event_from_scene(const QPointF &_a); \

namespace painters {

    enum class p_component_type {
        None = 0,
        // below for gif class
        Brush,
        Rect,
        // for alpha channel.
        Pixmap,
        // other component.
        Image
    };

    /*!
     * @brief the basic object. No inherent from only qy based class.
     *
     * Provide various
     */
    class p_component_obj {
    public:
        p_component_obj(p_component_type rhs): m_type(rhs) {};
        p_component_obj(p_component_type rhs, const std::string &a): m_type(rhs), m_debug_name(std::move(a)) {};
        virtual ~p_component_obj() = default;

        void set_name(const std::string &rhs) { m_debug_name = rhs; }
        p_component_type get_type() { return m_type; }
        const std::string &get_name() { return m_debug_name; }

    private:
        p_component_type    m_type;
        std::string         m_debug_name;
    };

    class p_brush_component: public p_component_obj, public QGraphicsPathItem {
    public:
        p_brush_component(const std::string &name= "brush"):
            p_component_obj(p_component_type::Brush, name),
            QGraphicsPathItem(nullptr), m_color(0, 128, 255,  60) {
            m_dirty = false;
            m_pen.setColor(m_color);
            m_pen.setWidth(10);
            m_pen.setStyle(Qt::PenStyle::SolidLine);
            setPen(m_pen);
            setPath(m_path);
            __init__();
        };

    OVERLOAD_EVENT: ///< below is the inner call, just when this component is selected.
        OVERLOAD_FUNC;

    public:
        void __init__();
        QPen &get_pen() { return m_pen; }
        QColor &get_color() { return m_color; }
        bool is_dirty() { return m_dirty; }

        void set_pen(QPen &pen) { m_pen = pen; }
        void set_color(QColor &color) { m_color= color, m_pen.setColor(color); }
        void set_pen_width(int width) { m_pen.setWidth(width); }
        void set_pen_style(Qt::PenStyle style) { m_pen.setStyle(style); }

        void set_selectable(bool enable);
        void set_movable(bool enable);

    EVENT_FROM_SCENE:
        EVENT_FROM_SCENE_FUNC;

    private: ///< interpolation for m_points.
        void _interpolation_(uint32_t samples);

    private:
        bool                m_dirty; ///< To determine if this component has be draw or not.
        bool                m_interpolation_enable = true;
        QPointF             m_start_point;
        QPointF             m_end_point;
        QPen                m_pen;
        QColor              m_color;
        QPainterPath        m_path;
        QVector<QPointF>    m_points;
        QSize               m_bounding_box_size;
    };

    class p_pixmap_component: public p_component_obj, public QGraphicsPixmapItem {
    public:
        p_pixmap_component(const std::string &name= "pixmap"):
            p_component_obj(p_component_type::Pixmap, name),
            QGraphicsPixmapItem(nullptr) {
            // initialize. TODO
        }
    private:


    };

    class p_rect_component: public p_component_obj, public QGraphicsRectItem {
    public:
        p_rect_component(const std::string &name= "Rect"):
            p_component_obj(p_component_type::Rect, name),
            QGraphicsRectItem(nullptr),
            m_color_out(0, 32, 128, 40),
            m_color_inner(0, 128, 32, 40) {
            // initialize. TODO
        }

    OVERLOAD_EVENT:
        OVERLOAD_FUNC;

    EVENT_FROM_SCENE:
        EVENT_FROM_SCENE_FUNC;

    public: ///< get and settings
        bool is_dirty() { return m_dirty; }
        const QPointF &get_center_point_const() { return m_center_point; }
        const QPointF &get_start_point_const() { return m_start_point; }
        const QPointF &get_end_point_const() { return m_end_point; }
        QPointF &get_center_point() { return m_center_point; }
        QPointF &get_start_point() { return m_start_point; }
        QPointF &get_end_point() { return m_end_point; }

        const QColor &get_color_inner_const() { return m_color_inner; }
        const QColor &get_color_out_const() { return m_color_out; }
        QColor &get_color_inner() { return m_color_inner; }
        QColor &get_color_out() { return m_color_out; }

        void set_color_inner(const QColor &a) {
            m_color_inner = a;
            m_brush.setColor(m_color_inner);
            setBrush(m_brush);
        }
        void set_color_out(const QColor &a) {
            m_color_out = a;
            m_pen_out.setColor(m_color_out);
            setPen(m_pen_out);
        }

        const QBrush &get_brush_const() { return m_brush; }
        const QPen &get_pen_out_const() { return m_pen_out; }
        QBrush &get_brush() { return m_brush; }
        QPen &get_pen_out() { return m_pen_out; }

    private:
        bool        m_dirty = false;
        QPointF     m_center_point;
        QPointF     m_start_point;
        QPointF     m_end_point;
        QPen        m_pen_out;
        QColor      m_color_out;
        QBrush      m_brush;
        QColor      m_color_inner;
    };


}; //! namespace painters

#endif // P_COMPONENT_H
