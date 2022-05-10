#ifndef P_GRAPHICVIEWPORT_H
#define P_GRAPHICVIEWPORT_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"
#include <QGraphicsView>

namespace painters {

    class p_graphic_view: public QGraphicsView {
    public:
        p_graphic_view(QWidget *parent= nullptr):
            QGraphicsView(parent) {

        }

    protected: ///< overide the mouse event from graphic view
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

    private: ///< for zoom and transform
        void zoom_in();
        void zoom_out();
        void transform(const QPointF &rhs);


    public: ///< for flags

    public: ///< for private value setting and get.
        void set_move_speed(double a) { m_move_speed = a; }
        void set_scale_speed(double a) { m_scale_speed = a; }

        double get_move_speed() { return m_move_speed; }
        double get_scale_speed() { return m_scale_speed; }
        const QPointF &get_cur_point() { return m_cur_point; }
        const QPointF &get_last_point() { return m_last_point; }

    private:
        double      m_move_speed;
        double      m_scale_speed;
        QPointF     m_cur_point;
        QPointF     m_last_point;
    };

}

#endif // P_GRAPHICVIEWPORT_H
