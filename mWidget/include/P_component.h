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

namespace painters {

    enum class p_component_type {
        None = 0,
        // below for gif class
        Brush,
        Shape,
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
            QGraphicsPathItem(nullptr), m_color(0, 128, 255) {
            m_dirty = false;
            m_pen.setColor(m_color);
            m_pen.setWidth(10);
            m_pen.setStyle(Qt::PenStyle::SolidLine);
            setPen(m_pen);
            setPath(m_path);
            __init__();
        };

    protected: ///< below is the inner call, just when this component is selected.
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    public:
        void __init__();
        QPen &get_pen() { return m_pen; }
        QColor &get_color() { return m_color; }

        void set_pen(QPen &pen) { m_pen = pen; }
        void set_color(QColor &color) { m_color= color, m_pen.setColor(color); }
        void set_pen_width(int width) { m_pen.setWidth(width); }
        void set_pen_style(Qt::PenStyle style) { m_pen.setStyle(style); }

        void set_selectable(bool enable);
        void set_movable(bool enable);

    public: ///< below for scene to recall. It is a global call
        void move_event_from_scene(const QPointF &_a);
        void press_event_from_scene(const QPointF &_a);
        void release_event_from_scene(const QPointF &_a);

    private: ///< interpolation for m_points.
        void _interpolation_(uint32_t samples);

    private:
        bool                m_dirty; ///< To determine if this component has be draw or not.
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

}; //! namespace painters

#endif // P_COMPONENT_H
