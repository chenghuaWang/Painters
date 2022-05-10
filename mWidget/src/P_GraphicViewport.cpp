#include "P_GraphicViewport.h"

namespace painters {

void p_graphic_view::mousePressEvent(QMouseEvent *event) {
    if (event->modifiers() == Qt::ShiftModifier) {
        m_transform_enable = true;
        m_last_point = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}
void p_graphic_view::mouseReleaseEvent(QMouseEvent *event){
    if (event->modifiers() == Qt::ShiftModifier) {
        m_transform_enable = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void p_graphic_view::mouseMoveEvent(QMouseEvent *event){
    if (m_transform_enable){
        QPointF mouse_delta = mapToScene(event->pos()) - mapToScene(m_last_point);
        transform_m(mouse_delta);
    }

    m_last_point = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void p_graphic_view::wheelEvent(QWheelEvent *event){
    int32_t y_offset = event->angleDelta().y();
    y_offset > 0 ? zoom_in() : zoom_out();
}

void p_graphic_view::keyPressEvent(QKeyEvent *event){
    QGraphicsView::keyPressEvent(event);
}

void p_graphic_view::zoom(float scale_rate) {
    scale(scale_rate, scale_rate);
    m_scaled *= scale_rate;
}

void p_graphic_view::zoom_in() {
    zoom(1 + m_scale_delta);
}

void p_graphic_view::zoom_out() {
    zoom(1 - m_scale_delta);
}

void p_graphic_view::transform_m(QPointF rhs) {
    rhs *= m_scaled;
    rhs *= m_move_speed;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint new_center(viewport()->rect().width() / 2 - rhs.x(),  viewport()->rect().height() / 2 - rhs.y());
    centerOn(mapToScene(new_center));

    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

}
