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

#include <QIcon>
#include <QtMath>
#include <QImage>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#define OVERLOAD_EVENT protected
#define OVERLOAD_FUNC \
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override; \
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; \
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; \
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;\


#define OVERLOAD_FUNC_IMPL(name, base_class) \
    void name::mouseMoveEvent(QGraphicsSceneMouseEvent *event) { \
        if ((event->modifiers() == Qt::AltModifier && isSelected()) || (isSelected() && p_canvas::get_instance()->get_tool_type() == tool_type::Select)) { \
            QGraphicsItem::mouseMoveEvent(event); \
        } \
        else if (event->modifiers() == Qt::ControlModifier && isSelected() && m_resize) { \
        } \
    } \
    void name::mousePressEvent(QGraphicsSceneMouseEvent *event) {\
        if (event->button() == Qt::LeftButton) {\
            if (event->modifiers() == Qt::AltModifier) {\
                setSelected(true);\
            }\
            else if (event->modifiers() == Qt::ShiftModifier) {\
            } \
            else if (event->modifiers() == Qt::ControlModifier) { \
            }\
        }\
        else {\
            event->ignore();\
        }\
    }\
    void name::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {\
        QGraphicsItem::mouseReleaseEvent(event); \
    }\
    void name::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {\
        {\
            if (isSelected()) {\
                qreal scale_factor = abs(painter->transform().m11());\
                scale_factor = qMax(scale_factor, 0.85);\
                QTransform trans_previous = this->transform();\
                this->resetTransform();\
                QRectF bound_rect = boundingRect();\
                QPen pen;\
                pen.setWidth(1);\
                pen.setColor(QColor(0, 64, 243, 60));\
                pen.setStyle(Qt::DashLine);\
                painter->setPen(pen);\
                QRectF _out_line_ = bound_rect.adjusted(-m_payload.m_interval / scale_factor,\
                                                        -m_payload.m_interval / scale_factor,\
                                                        m_payload.m_interval / scale_factor,\
                                                        m_payload.m_interval / scale_factor);\
                painter->drawRect(_out_line_);\
                painter->setPen(Qt::NoPen);\
                painter->setBrush(QColor(0, 100, 200, 60));\
                painter->drawEllipse(_out_line_.bottomRight(),\
                                     m_payload.m_active_area_r / scale_factor,\
                                     m_payload.m_active_area_r / scale_factor);\
                painter->drawPixmap(QRect(_out_line_.bottomRight().x() - m_payload.m_active_area_r / (2*scale_factor),\
                                          _out_line_.bottomRight().y() - m_payload.m_active_area_r / (2*scale_factor),\
                                          m_payload.m_active_area_r / scale_factor,\
                                          m_payload.m_active_area_r / scale_factor),\
                                    m_payload.m_resize_pixmap);\
                painter->drawEllipse(_out_line_.topRight(),\
                                     m_payload.m_active_area_r / scale_factor,\
                                     m_payload.m_active_area_r / scale_factor);\
                painter->drawPixmap(QRect(_out_line_.topRight().x() - m_payload.m_active_area_r / (2*scale_factor),\
                                          _out_line_.topRight().y() - m_payload.m_active_area_r / (2*scale_factor),\
                                          m_payload.m_active_area_r / scale_factor,\
                                          m_payload.m_active_area_r / scale_factor),\
                                    m_payload.m_close_pixmap);\
                painter->drawEllipse(_out_line_.bottomLeft(),\
                                     m_payload.m_active_area_r / scale_factor,\
                                     m_payload.m_active_area_r / scale_factor);\
                painter->drawPixmap(QRect(_out_line_.bottomLeft().x() - m_payload.m_active_area_r / (2*scale_factor),\
                                          _out_line_.bottomLeft().y() - m_payload.m_active_area_r / (2*scale_factor),\
                                          m_payload.m_active_area_r / scale_factor,\
                                          m_payload.m_active_area_r / scale_factor),\
                                    m_payload.m_rotate_pixmap);\
                this->setTransform(trans_previous);\
            }\
        }\
        base_class::paint(painter, option, widget);\
    }\



#define EVENT_FROM_SCENE public
#define EVENT_FROM_SCENE_FUNC \
    void move_event_from_scene(const QPointF &_a); \
    void press_event_from_scene(const QPointF &_a); \
    void release_event_from_scene(const QPointF &_a); \

namespace painters {
    enum class p_op_type {
        None = 0,
        Move,
        Resize,
        Rotate,
        Text
    };

    struct p_op_payload {
        p_op_payload() {
            m_close_icon.load(":/icon/icons8-close-96.png");
            m_resize_icon.load(":/icon/icons8-resize-96.png");
            m_rotate_icon.load(":/icon/icons8-rotate-left-96.png");

            __init_pixmap__();
        }

        ~p_op_payload() = default;

        void __init_pixmap__() {
            m_close_pixmap = QPixmap::fromImage(m_close_icon);
            m_resize_pixmap = QPixmap::fromImage(m_resize_icon);
            m_rotate_pixmap = QPixmap::fromImage(m_rotate_icon);
        }

        QPointF     m_center_point;
        QPointF     m_bounding_box_size; ///< .rx for w, .ry for height.
        QPointF     m_reference_point;

        QPointF     m_scene_pos;
        QPointF     m_local_pos;
        QPointF     m_cur_pos;

        int         m_active_area_r = 16; ///< used in circle. r.
        int         m_interval = 4;///< interval between item and bounding box.

        p_op_type   m_op_type;

        static QImage   m_close_icon;
        static QImage   m_resize_icon;
        static QImage   m_rotate_icon;

        QPixmap  m_close_pixmap;
        QPixmap  m_resize_pixmap;
        QPixmap  m_rotate_pixmap;

    public:
        qreal cauculate_distance(const QPointF &_a, const QPointF &_b) {
            qreal x = _a.x() - _b.x();
            qreal y = _a.y() - _b.y();
            return qSqrt(x*x + y*y);
        }
    };
}

namespace painters {

    enum class p_component_type {
        None = 0,
        // below for gif class
        Brush,
        Rect,
        Circle,
        // for alpha channel.
        Pixmap,
        // other component.
        Image,
        Text
    };

    /*!
     * @brief the basic object. No inherent from only qy based class.
     *
     * Provide various
     */
    class p_component_obj {
    public:
        p_component_obj(p_component_type rhs):
            m_type(rhs) {
        };

        p_component_obj(p_component_type rhs, const std::string &a): m_type(rhs), m_debug_name(std::move(a)) {};
        virtual ~p_component_obj() = default;

        void set_name(const std::string &rhs) { m_debug_name = rhs; }
        p_component_type get_type() { return m_type; }
        const std::string &get_name() { return m_debug_name; }

        bool                m_resize = false;
        bool                m_lazy_remove = false; ///< important!! for delate remove.!!!

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

        p_brush_component(QGraphicsPathItem *rhs, const std::string &name= "brush"):
            p_component_obj(p_component_type::Brush, name),
            QGraphicsPathItem(rhs) {
            m_dirty = false;
        }

        void init_from_base_item(QGraphicsPathItem* item) {
            this->m_path = item->path();
            this->setPath(item->path());
            this->setPen(item->pen());
            this->setPos(item->pos());
            this->setTransform(item->transform());
            this->setScale(item->scale());
            this->__init__();
            this->setX(item->x());
            this->setY(item->y());

            // noneed to set origin to center bounding box.
            // previous project item has already did it.

            this->prepareGeometryChange();
            setPath(m_path);
            this->update(); // redraw
            m_dirty = true;
        }

    OVERLOAD_EVENT: ///< below is the inner call, just when this component is selected.
        OVERLOAD_FUNC;

    public:
        p_op_payload m_payload;

    public:
        void __init__();
        QColor &get_color() { return m_color; }
        bool is_dirty() { return m_dirty; }
        void set_dirty(bool enable) { m_dirty = enable; }

        void set_pen(QPen &pen) { m_pen = pen; setPen(m_pen); }
        void set_color(QColor &color) { m_color= color, m_pen.setColor(color); setPen(m_pen);}
        void set_pen_width(int width) { m_pen.setWidth(width); }
        void set_pen_style(Qt::PenStyle style) { m_pen.setStyle(style); }

        void set_selectable(bool enable);
        void set_movable(bool enable);

        void set_interpolation(bool enable) { m_interpolation_enable = enable; }

        const QPen &get_pen_const() { return m_pen; }
        QPen &get_pen() { return m_pen; }

    EVENT_FROM_SCENE:
        EVENT_FROM_SCENE_FUNC;

    private: ///< interpolation for m_points.
        void _interpolation_(uint32_t samples);

    private:
        bool                m_dirty; ///< To determine if this component has be draw or not.
        bool                m_interpolation_enable = false;
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
            // initialize.
            m_pen_out.setColor(m_color_out);
            m_pen_out.setWidth(4);
            m_brush.setColor(m_color_inner);
            m_brush.setStyle(Qt::SolidPattern);
            setPen(m_pen_out);
            setBrush(m_brush);
            __init__();
        }

        void init_from_base_item(QGraphicsRectItem* item) {
            this->setBrush(item->brush());
            this->setRect(item->rect());
            this->setPen(item->pen());
            this->setPos(item->pos());
            this->setTransform(item->transform());
            this->setScale(item->scale());
            this->__init__();
            this->setX(item->x());
            this->setY(item->y());

            // noneed to set origin to center bounding box.
            // previous project item has already did it.

            this->prepareGeometryChange();
            this->update(); // redraw
            m_dirty = true;
        }

        void __init__();

    OVERLOAD_EVENT:
        OVERLOAD_FUNC;

    EVENT_FROM_SCENE:
        EVENT_FROM_SCENE_FUNC;

    public:
        p_op_payload m_payload;

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

        void set_brush(QBrush &_a) { setBrush(_a); }
        void set_pen(QPen &_a) { setPen(_a); }

        const QBrush &get_brush_const() { return m_brush; }
        const QPen &get_pen_out_const() { return m_pen_out; }
        QBrush &get_brush() { return m_brush; }
        QPen &get_pen_out() { return m_pen_out; }

    private:
        std::pair<QPointF, QPointF> __get_rect_payload__();

    private:
        bool        m_dirty = false;
        QPointF     m_center_point;
        QPointF     m_start_point;
        QPointF     m_end_point;
        QPen        m_pen_out;
        QColor      m_color_out;
        QBrush      m_brush;
        QColor      m_color_inner;
        QSize       m_bounding_box_size;
    };

    class p_circle_component: public p_component_obj, public QGraphicsEllipseItem {
    public:
        p_circle_component(const std::string &name= "circle"):
            p_component_obj(p_component_type::Circle, name),
            QGraphicsEllipseItem(nullptr),
            m_color_out(0, 128, 32, 40),
            m_color_inner(0, 128, 32, 40){
            m_pen_out.setColor(m_color_out);
            m_brush.setColor(m_color_inner);
            m_brush.setStyle(Qt::BrushStyle::SolidPattern);
            m_pen_out.setStyle(Qt::PenStyle::SolidLine);
            setPen(m_pen_out);
            setBrush(m_brush);
            __init__();
            m_dirty = false;
        }

        void init_from_base_item(QGraphicsEllipseItem* item) {
            this->setBrush(item->brush());
            this->setRect(item->rect());
            this->setPen(item->pen());
            this->setPos(item->pos());
            this->setTransform(item->transform());
            this->setScale(item->scale());
            this->__init__();
            this->setX(item->x());
            this->setY(item->y());

            // noneed to set origin to center bounding box.
            // previous project item has already did it.

            this->prepareGeometryChange();
            this->update(); // redraw
            m_dirty = true;
        }

        void __init__();

        void set_brush(QBrush &_a) { setBrush(_a); }
        void set_pen(QPen &_a) { setPen(_a); }

    OVERLOAD_EVENT:
        OVERLOAD_FUNC;

    EVENT_FROM_SCENE:
        EVENT_FROM_SCENE_FUNC;

    public:
        p_op_payload m_payload;

    public:
        const QColor &get_color_out_const() { return m_color_out; }
        QColor &get_out_color() { return m_color_out; }

        const QColor &get_color_inner_const() { return m_color_inner; }
        QColor &get_inner_color() { return m_color_inner; }

        const QPen &get_pen_const() { return m_pen_out; }
        QPen &get_pen() { return m_pen_out; }

        const QBrush &get_brush_const() { return m_brush; }
        QBrush &get_brush() { return m_brush; }

        void set_color_out(const QColor &rhs) {
            m_color_out = rhs;
            m_pen_out.setColor(m_color_out);
        }

        void set_color_inner(const QColor &rhs) {
            m_color_inner = rhs;
            m_pen_out.setColor(m_color_inner);
        }

    private:
        std::pair<QPointF, QPointF> __get_rect_payload__();

    private:
        bool        m_dirty = false;
        QPointF     m_center_point;
        qreal       m_r;
        QColor      m_color_out;
        QPen        m_pen_out;
        QBrush      m_brush;
        QColor      m_color_inner;

        QPointF     m_start_point;
        QPointF     m_end_point;
    };

    class p_line_component: public p_component_obj, public QGraphicsLineItem {
    public:

    private:
    };

    class p_image_component: public p_component_obj, public QGraphicsPixmapItem {
    public:
        p_image_component(const std::string &name= "image"):
            p_component_obj(p_component_type::Image, name),
            QGraphicsPixmapItem(nullptr),
            m_alpha_channel(255) {
            __init__();
        }
        p_image_component(const std::string &file_path, const std::string &name= "image"):
            p_component_obj(p_component_type::Image, name),
            QGraphicsPixmapItem(nullptr),
            m_alpha_channel(255),
            m_file_path(file_path){
            m_pixmap.load(file_path.c_str());
            setPixmap(m_pixmap);
            this->update();
            __init__();
        }

        void load_image(const std::string &file_name) {
            m_pixmap.load(file_name.c_str());
            setPixmap(m_pixmap);
            this->update();
        }

        void init_from_base_item(QGraphicsPixmapItem* item) {
            this->setPixmap(item->pixmap());
            this->setPos(item->pos());
            this->setTransform(item->transform());
            this->setScale(item->scale());
            this->__init__();
            this->setX(item->x());
            this->setY(item->y());

            // noneed to set origin to center bounding box.
            // previous project item has already did it.

            this->prepareGeometryChange();
            this->update(); // redraw
//            m_dirty = true;
        }


    private:
        void __init__();

    OVERLOAD_EVENT:
        OVERLOAD_FUNC;

    EVENT_FROM_SCENE:
        EVENT_FROM_SCENE_FUNC;

    public:
        p_op_payload m_payload;

    private:
        uint32_t            m_alpha_channel;
        std::string         m_file_path;
        QPixmap             m_pixmap;
    };

    class p_text_component: public p_component_obj, public QGraphicsTextItem {
    public:
        p_text_component(const std::string &name= "text"):
            p_component_obj(p_component_type::Text, name),
            QGraphicsTextItem(nullptr) {
            __init__();
        }

    OVERLOAD_EVENT:
        OVERLOAD_FUNC;

    EVENT_FROM_SCENE:
        EVENT_FROM_SCENE_FUNC;

    public:
        p_op_payload m_payload;

    private:
        void __init__();

    private:
        std::string         m_text;
    };

}; //! namespace painters

#endif // P_COMPONENT_H
