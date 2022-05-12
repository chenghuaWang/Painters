#include "p_inspector.h"
#include <QColorDialog>

namespace painters {

void p_brush_inspector::__init__() {
    m_r_slider->setValue(m_color.red());
    m_g_slider->setValue(m_color.green());
    m_b_slider->setValue(m_color.blue());
    m_a_slider->setValue(m_color.alpha());
    m_brush_type_combobox->setCurrentIndex(0);
    m_brush_thickness_spinbox->setValue(m_pen.widthF());

    QPainterPath tmp;

    tmp.moveTo(100, 40);
    for (int i = 1; i < 5; ++i) {
        tmp.lineTo(40 + 60 * cos(0.8 * i * M_PI), 40 + 60 * sin(0.8 * i * M_PI));
    }
    tmp.closeSubpath();

    m_path_item.setPath(tmp);
    m_path_item.setPen(m_pen);

    m_graphic_scene->addItem(&m_path_item);
    m_graphic_scene->update();

    m_graphic_view->setBackgroundBrush(Qt::white);
    m_graphic_view->setScene(m_graphic_scene.get());

    // init combobox type.
    m_brush_type_combobox->addItem("SolidLine");
    m_brush_type_combobox->addItem("DotDashLine");
    m_brush_type_combobox->addItem("DotLine");
    m_brush_type_combobox->addItem("DashLine");

    m_brush_cap_combobox->addItem("FlatCap");
    m_brush_cap_combobox->addItem("RoundCap");
    m_brush_cap_combobox->addItem("SquareCap");
    m_brush_cap_combobox->addItem("MPenCapStyle");

    m_brush_join_combobox->addItem("BevelJoin");
    m_brush_join_combobox->addItem("MPenJoinStyle");
    m_brush_join_combobox->addItem("MiterJoin");
    m_brush_join_combobox->addItem("RoundJoin");
    m_brush_join_combobox->addItem("SvgMiterJoin");

    // init slider
    m_r_slider->setMaximum(255);
    m_r_slider->setMinimum(0);
//    m_r_slider->setTracking(false);
    m_g_slider->setMaximum(255);
    m_g_slider->setMinimum(0);
//    m_g_slider->setTracking(false);
    m_b_slider->setMaximum(255);
    m_b_slider->setMinimum(0);
//    m_b_slider->setTracking(false);
    m_a_slider->setMaximum(255);
    m_a_slider->setMinimum(0);
//    m_a_slider->setTracking(false);

    // below for signal and slots binding.
    connect(this->m_choose_button.get(), SIGNAL(clicked()), this, SLOT(slots_choose_color())); ///< button
    connect(this->m_r_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(slots_change_slider_r(int))); ///< slider r
    connect(this->m_g_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(slots_change_slider_g(int))); ///< slider g
    connect(this->m_b_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(slots_change_slider_b(int))); ///< slider b
    connect(this->m_a_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(slots_change_slider_a(int))); ///< slider a
    connect(this->m_brush_thickness_spinbox.get(), SIGNAL(valueChanged(qreal)), this, SLOT(slots_change_doublespin_box(qreal)));
    connect(this->m_brush_type_combobox.get(), SIGNAL(currentTextChanged(const QString&)), this, SLOT(slots_change_combobox_style(const QString&)));
    connect(this->m_brush_cap_combobox.get(), SIGNAL(currentTextChanged(const QString&)), this, SLOT(slots_change_combobox_cap_style(const QString&)));
    connect(this->m_brush_join_combobox.get(), SIGNAL(currentTextChanged(const QString&)), this, SLOT(slots_change_combobox_join_style(const QString&)));

    // below for init emit
    emit signal_pen_changed(m_pen);
}

void p_brush_inspector::__redraw__() {
    m_path_item.setPen(m_pen);
    m_graphic_scene->update();
}

void p_brush_inspector::slots_choose_color() {
    m_color = QColorDialog::getColor(Qt::white, this, "Color Dialog");
    m_pen.setColor(m_color);
    m_r_slider->setValue(m_color.red());
    m_g_slider->setValue(m_color.green());
    m_b_slider->setValue(m_color.blue());
    m_a_slider->setValue(m_color.alpha());
    __redraw__();
    emit signal_pen_changed(m_pen);
}

void p_brush_inspector::slots_change_slider_r(int val) {
    m_color.setRed(val);
    m_pen.setColor(m_color);
    __redraw__();
    emit signal_pen_changed(m_pen);
}
void p_brush_inspector::slots_change_slider_g(int val) {
    m_color.setGreen(val);
    m_pen.setColor(m_color);
    __redraw__();
    emit signal_pen_changed(m_pen);
}
void p_brush_inspector::slots_change_slider_b(int val) {
    m_color.setBlue(val);
    m_pen.setColor(m_color);
    __redraw__();
    emit signal_pen_changed(m_pen);
}
void p_brush_inspector::slots_change_slider_a(int val) {
    m_color.setAlpha(val);
    m_pen.setColor(m_color);
    __redraw__();
    emit signal_pen_changed(m_pen);
}

void p_brush_inspector::slots_change_doublespin_box(qreal val) {
    m_pen.setWidthF(val);
    __redraw__();
    emit signal_pen_changed(m_pen);
}

void p_brush_inspector::slots_change_combobox_style(const QString& _a) {
    m_pen.setStyle(map_to_type(_a));
    __redraw__();
    emit signal_pen_changed(m_pen);
}

void p_brush_inspector::slots_change_combobox_cap_style(const QString& _a) {
    m_pen.setCapStyle(map_to_cap_type(_a));
    __redraw__();
    emit signal_pen_changed(m_pen);
}

void p_brush_inspector::slots_change_combobox_join_style(const QString& _a) {
    m_pen.setJoinStyle(map_to_join_type(_a));
    __redraw__();
    emit signal_pen_changed(m_pen);
}

}; //! namespace painters;

namespace painters {

void p_layer_inspector::__init__() {
    m_alpha_spinbox->setMaximum(1.0);
    m_alpha_spinbox->setMinimum(0.0);
    m_alpha_enable_checkbox->setChecked(true);
    m_lock_checkbox->setChecked(false);
}

void p_layer_inspector::slots_update_layer_widget(REF(p_graphic_layer)& _a) {
    m_name_editor->setText(_a->get_name().c_str());
    QString tmp(std::string(std::to_string(_a->get_w()) + " x " + std::to_string(_a->get_h())).c_str());
    m_size_editor->setText(tmp);
}

}; //namespace painters
