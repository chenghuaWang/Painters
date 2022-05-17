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
#include "P_layer_manage.h"

#include <QGraphicsScene>
#include <QMessageBox>

///< TODO add brush counter.
#define MOUSE_EVENT_PRESS(object_ptr, object_type, other_sentence) \
    if (event->button() == Qt::LeftButton && \
            event->modifiers() != Qt::AltModifier && \
            event->modifiers() != Qt::ShiftModifier) { \
        if (_a.x() < 0 || _a.x() > m_scene_size.width()) break; \
        if (_a.y() < 0 || _a.y() > m_scene_size.height()) break; \
        if (m_cur_choosed_layer->m_locked) { \
            QMessageBox::critical(nullptr, "Layer is locked", "free the layer before draw!!!"); \
            break; \
        } \
        object_ptr##_enable = true; \
        object_ptr = new object_type(__combine_name__(object_ptr##_cnt)); \
        other_sentence;\
        object_ptr->press_event_from_scene(_a); \
        addItem(object_ptr); \
        m_cur_choosed_layer->add_node(object_ptr->get_name(), object_ptr); \
    } \

#define MOUSE_EVENT_MOVE(object_ptr) \
    if (object_ptr##_enable || m_cur_tool == tool_type::Select) { \
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
        signal_update_layer_tree(); \
        emit signal_update_layer_tree(); \
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
        Q_OBJECT
    public:
        p_canvas() {
            setBackgroundBrush(Qt::white);
            setSceneRect(QRectF(0, 0, m_scene_size.width(), m_scene_size.height()));
            QPen pen;
            pen.setColor(QColor(0, 0, 0));
            pen.setStyle(Qt::PenStyle::DashDotLine);
            pen.setWidth(1);
            m_rectItem = new QGraphicsRectItem();
            m_rectItem->setRect(0, 0, 1024, 720);
            m_rectItem->setPen(pen);
            m_rectItem->setBrush(QBrush(QColor(255, 255, 255)));
            addItem(m_rectItem);

            // layer manager. Init the first layer.
            set_cur_choosed_layer(CREATE_REF(p_graphic_layer)("untitled layer 1"));
            m_cur_choosed_layer->m_w = 1024;
            m_cur_choosed_layer->m_h = 720;
            m_cur_choosed_layer->set_zbuffer(0);
            m_layer_stack.push_layer(m_cur_choosed_layer);

            m_s_instance = this;
        }

        ~p_canvas() {
            delete m_reference_line;
            delete m_reference_line_v;
        }

    private:
        std::string __combine_name__(uint32_t &a) {
            switch (m_cur_tool) {
            case tool_type::Circle:
                return "Circle " + std::to_string(++a);
                break;
            case tool_type::Image:
                return "Image " + std::to_string(++a);
                break;
            case tool_type::Pen:
                return "Pen " + std::to_string(++a);
                break;
            case tool_type::Rect:
                return "Rect " + std::to_string(++a);
                break;
            default:
                break;
            }
            return "None " + std::to_string(++a);
        }

    protected: ///< overide original virtual functions.
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    public: ///< Interface to other class.
        void set_tool_type(tool_type a) { m_cur_tool = a; }
        tool_type get_tool_type() { return m_cur_tool; }
        bool get_cur_brush_enable() { return m_cur_brush_enable; }
        bool get_rect_brush_enable() { return m_cur_rect_enable; }
        bool get_circle_brush_enable() { return m_cur_circle_enable; }
        bool get_cur_image_enable() { return m_cur_image_enable; }

        p_brush_component *get_brush_component() { return m_cur_brush; }
        p_rect_component *get_rect_component() { return m_cur_rect; }
        p_circle_component *get_circle_component() { return m_cur_circle; }
        p_image_component *get_image_component() { return m_cur_image; }

        const std::string get_cur_image_string() { return m_cur_image_string; }
        void set_cur_image_string(const std::string &_a) { m_cur_image_string = _a; }

        void add_layer(const std::string &_a= "") {
            std::string tmp = _a;
            if (_a == ""){
                tmp = "Layer " + std::to_string(++m_cur_layer_cnt);
            }
            m_cur_choosed_layer = CREATE_REF(p_graphic_layer)(tmp);
            m_layer_stack.push_layer(m_cur_choosed_layer);
        }
        void delete_layer(REF(p_graphic_layer) &_a) {
            m_layer_stack.pop_layer(_a);
        }
        void rename_layer(const std::string &_a) {
            m_cur_choosed_layer->rename(_a);
        }

        void set_cur_choosed_layer(const REF(p_graphic_layer)& _a);

    signals:
        void signal_update_layer_tree();

    public slots:
        void slots_brush_pen_changed(QPen &_a);
        void slots_process_reference(const QString& _a);
        void slots_draw_reference_line();
        void slots_delete_refernce_line();
        void slots_lock_cur_layer(bool enable);

        void slots_set_tool_type_shape(bool enable);
        void slots_set_tool_type_pen(bool enable);
        void slots_set_tool_type_image(bool enable);

        void slots_shape_brush_changed(QBrush &_a);
        void slots_shape_pen_changed(QPen &_a);

        void slots_image_string_changed(const QString &_a);

    private: ///< numerous flags setting.
        tool_type           m_cur_tool = tool_type::Pen;
        bool                m_cur_brush_enable = false;
        bool                m_cur_rect_enable = false;
        bool                m_cur_circle_enable = false;
        bool                m_cur_image_enable = false;

    private:
        QPen                m_cur_brush_pen;
        QPen                m_cur_rect_pen;
        QPen                m_cur_circle_pen;
        QBrush              m_cur_rect_brush;
        QBrush              m_cur_circle_brush;

    private: ///< scene global data
        QSize               m_scene_size = QSize(1024, 720);

    private: ///< temporary data
        p_brush_component   *m_cur_brush = nullptr;
        p_rect_component    *m_cur_rect = nullptr;
        p_circle_component  *m_cur_circle =  nullptr;
        p_image_component   *m_cur_image = nullptr;
        std::string         m_cur_image_string;

    private: ///< num of every component.
        uint32_t            m_cur_brush_cnt = 0;
        uint32_t            m_cur_rect_cnt = 0;
        uint32_t            m_cur_circle_cnt = 0;
        uint32_t            m_cur_image_cnt = 0;
        uint32_t            m_cur_none_cnt = 0;
        uint32_t            m_cur_layer_cnt = 0;

    public: ///< process reference line
        QGraphicsPathItem   *m_reference_line = nullptr;
        QGraphicsPathItem   *m_reference_line_v = nullptr;
        QGraphicsRectItem   *m_rectItem = nullptr;

        QGraphicsItem       *m_choosed_for_select = nullptr;
        p_op_type           m_op_type = p_op_type::None;
        qreal cauculate_distance(const QPointF &_a, const QPointF &_b) {
            qreal x = _a.x() - _b.x();
            qreal y = _a.y() - _b.y();
            return qSqrt(x*x + y*y);
        }

    public: /// Layer manager
        REF(p_graphic_layer)    m_cur_choosed_layer;
        p_graphic_layer_stack   m_layer_stack;

    public:
        static p_canvas         *m_s_instance;
        static p_canvas*        get_instance();
    };

}

#endif // P_CANVASWIDGET_H
