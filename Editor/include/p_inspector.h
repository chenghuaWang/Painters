#ifndef P_INSPECTOR_H
#define P_INSPECTOR_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsItem>

#define REGISTER_FUNC(object_name, widget_name) \
    void set_##object_name(widget_name *_a) { \
        m_##object_name = REF(widget_name)(_a); \
    } \


namespace painters {

    class p_inspector: public QWidget {
        Q_OBJECT
    public:
        p_inspector(const std::string &name):m_debug_name(name) {};
        ~p_inspector() = default;

        std::string m_debug_name;
    };

    class p_brush_inspector: public p_inspector {
        Q_OBJECT
    public:
        p_brush_inspector(): p_inspector("brush inspector") {
            m_graphic_scene = CREATE_REF(QGraphicsScene)();
            m_color = QColor(0, 128, 32, 40);

            m_pen.setColor(m_color);
            m_pen.setStyle(Qt::PenStyle::SolidLine);
            m_pen.setWidth(10);
        }

    public:
        REGISTER_FUNC(brush_type_combobox, QComboBox);
        REGISTER_FUNC(brush_thickness_spinbox, QDoubleSpinBox);
        REGISTER_FUNC(r_slider, QSlider);
        REGISTER_FUNC(g_slider, QSlider);
        REGISTER_FUNC(b_slider, QSlider);
        REGISTER_FUNC(a_slider, QSlider);
        REGISTER_FUNC(graphic_view, QGraphicsView);
        REGISTER_FUNC(graphic_scene, QGraphicsScene);
        REGISTER_FUNC(choose_button, QPushButton);

    public:
        void __init__();
        void __redraw__();

    signals:
        void signal_pen_changed(QPen&);

    public slots:
        void slots_choose_color();
        void slots_change_slider_r(int val);
        void slots_change_slider_g(int val);
        void slots_change_slider_b(int val);
        void slots_change_slider_a(int val);

    public:
        QColor              m_color;
        QPen                m_pen;

    public:
        REF(QComboBox)      m_brush_type_combobox = nullptr;
        REF(QDoubleSpinBox) m_brush_thickness_spinbox = nullptr;
        REF(QSlider)        m_r_slider = nullptr;
        REF(QSlider)        m_g_slider = nullptr;
        REF(QSlider)        m_b_slider = nullptr;
        REF(QSlider)        m_a_slider = nullptr;
        REF(QPushButton)    m_choose_button = nullptr;

        // below value, used in this class only.
        REF(QGraphicsView)  m_graphic_view = nullptr;
        REF(QGraphicsScene) m_graphic_scene = nullptr;
        QGraphicsPathItem   m_path_item;
    };

    class p_text_inspector: public p_inspector {

    };

    class p_image_inspector: public p_inspector {

    };

    class p_shape_inspector: public p_inspector {

    };

} //! namespace painters

#endif // P_INSPECTOR_H
