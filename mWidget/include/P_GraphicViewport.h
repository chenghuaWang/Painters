/*!
 * @author  chenghua.wang
 * @brief override the view, provide the movable and
 * scroable canvas.
 */

#ifndef P_GRAPHICVIEWPORT_H
#define P_GRAPHICVIEWPORT_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"
#include <QGraphicsView>
#include <QOpenGLWidget>

namespace painters {

    class p_graphic_view: public QGraphicsView {
        Q_OBJECT
    public:
        p_graphic_view(QWidget *parent= nullptr):
            QGraphicsView(parent) {
            ///< Use opengl vision 330. For accelerate.
            setViewport(new QOpenGLWidget(this));

            ///< If use cpu render mode. This anti-alisasing flags should be closed.
            setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            centerOn(0, 0);
        }

    protected: ///< overide the mouse event from graphic view
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

    private: ///< for zoom and transform
        void zoom(float scale_rate);
        void zoom_in();
        void zoom_out();
        void transform_m(QPointF rhs);

    public: ///< for flags
        bool        m_transform_enable = false;

    public: ///< for private value setting and get.
        void set_move_speed(double a) { m_move_speed = a; }
        void set_scale_speed(double a) { m_scale_speed = a; }

        double get_move_speed() { return m_move_speed; }
        double get_scale_speed() { return m_scale_speed; }
        const QPointF &get_cur_point() { return m_cur_point; }
        const QPoint  &get_last_point() { return m_last_point; }

    private:
        double      m_move_speed = 1.0;
        double      m_scale_delta = 0.1;
        double      m_scale_speed = 1.0;
        double      m_scaled = 1.0;
        QPointF     m_cur_point;
        QPoint      m_last_point;
    };

}

#endif // P_GRAPHICVIEWPORT_H
