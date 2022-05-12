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

#include <QLineEdit>
#include <QCheckBox>

#include "P_layer_manage.h"

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
            m_pen.setWidthF(8);
        }

    public:
        std::map<QString, Qt::PenStyle> string_map_type = {
            {"SolidLine", Qt::SolidLine},
            {"DotDashLine", Qt::DashDotDotLine},
            {"DotLine", Qt::DotLine},
            {"DashLine", Qt::DashLine}
        };

        std::map<QString, Qt::PenCapStyle> cap_string_map_type = {
            {"FlatCap", Qt::PenCapStyle::FlatCap},
            {"RoundCap", Qt::PenCapStyle::RoundCap},
            {"SquareCap", Qt::PenCapStyle::SquareCap},
            {"MPenCapStyle", Qt::PenCapStyle::MPenCapStyle}
        };

        std::map<QString, Qt::PenJoinStyle> join_string_map_type = {
            {"BevelJoin", Qt::PenJoinStyle::BevelJoin},
            {"MPenJoinStyle", Qt::PenJoinStyle::MPenJoinStyle},
            {"MiterJoin", Qt::PenJoinStyle::MiterJoin},
            {"RoundJoin", Qt::PenJoinStyle::RoundJoin},
            {"SvgMiterJoin", Qt::PenJoinStyle::SvgMiterJoin}
        };

        Qt::PenStyle map_to_type(const QString &_a) {
            return string_map_type[_a];
        }

        Qt::PenCapStyle map_to_cap_type(const QString &_a) {
            return cap_string_map_type[_a];
        }

        Qt::PenJoinStyle map_to_join_type(const QString &_a) {
            return join_string_map_type[_a];
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
        REGISTER_FUNC(brush_cap_combobox, QComboBox);
        REGISTER_FUNC(brush_join_combobox, QComboBox);

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
        void slots_change_doublespin_box(qreal val);
        void slots_change_combobox_style(const QString& _a);
        void slots_change_combobox_cap_style(const QString& _a);
        void slots_change_combobox_join_style(const QString& _a);

    public:
        QColor              m_color;
        QPen                m_pen;

    public:
        REF(QComboBox)      m_brush_type_combobox = nullptr;
        REF(QComboBox)      m_brush_cap_combobox = nullptr;
        REF(QComboBox)      m_brush_join_combobox = nullptr;
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

namespace painters {

    class p_layer_inspector: public p_inspector {
        Q_OBJECT
    public:
        p_layer_inspector(const std::string &name= "Layer inspector"): p_inspector(name) {}

        REGISTER_FUNC(name_editor, QLineEdit);
        REGISTER_FUNC(size_editor, QLineEdit);
        REGISTER_FUNC(alpha_spinbox, QDoubleSpinBox);
        REGISTER_FUNC(lock_checkbox, QCheckBox);
        REGISTER_FUNC(alpha_enable_checkbox, QCheckBox);

        REF(QLineEdit)      m_name_editor;
        REF(QLineEdit)      m_size_editor;
        REF(QDoubleSpinBox) m_alpha_spinbox;
        REF(QCheckBox)      m_lock_checkbox;
        REF(QCheckBox)      m_alpha_enable_checkbox;

        void __init__();

    public slots:
        void slots_update_layer_widget(REF(p_graphic_layer) &_a);
    };

} //! namespace painters.

#endif // P_INSPECTOR_H
