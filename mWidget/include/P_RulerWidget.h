/*!
 * @author chenghua.wang
 * @brief A ruler class for GraphicView
 * @note reference https://stackoverflow.com/questions/56588152/how-to-make-a-ruler-on-the-border-of-a-qgraphicsview
 */
#ifndef P_RULERWIDGET_H
#define P_RULERWIDGET_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"

namespace painters{

    /*!
     * @brief A Ruler widget. Self-defined for Graphic view
     * Ruler mapping feature.
     *
     * A naive approach. Draw ruler every time, may comsuming
     * memory.
     */
    class nQRulerWidget: public QWidget {
        Q_OBJECT
    public:

    protected:

    private:
        int offset;

    };
} //! namespace painters

#endif // P_RULERWIDGET_H
