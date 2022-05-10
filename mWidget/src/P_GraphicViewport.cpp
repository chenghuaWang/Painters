#include "P_GraphicViewport.h"

namespace painters {

void p_graphic_view::mousePressEvent(QMouseEvent *event) {

}
void p_graphic_view::mouseReleaseEvent(QMouseEvent *event){

}

void p_graphic_view::mouseMoveEvent(QMouseEvent *event){

}

void p_graphic_view::wheelEvent(QWheelEvent *event){
    int32_t y_offset = event->angleDelta().y();
    y_offset > 0 ? zoom_in() : zoom_out();
}

void p_graphic_view::keyPressEvent(QKeyEvent *event){

}

void p_graphic_view::zoom_in() {

}

void p_graphic_view::zoom_out() {

}

void p_graphic_view::transform(const QPointF &rhs) {

}

}
