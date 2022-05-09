/*!
 * @author  chenghua.wang
 * @brief   I use a scene to manage all component.
 * To better implment layers. I defined a layers
 * class, which actually is a node of the main-
 * layer-tree. The layer class contain multiple co-
 * mponent.
 */

#ifndef P_LAYER_MANAGE_H
#define P_LAYER_MANAGE_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"
#include <QGraphicsItem>
#include <QGraphicsScene>

namespace painters {

    class p_graphic_layer {
    public:

    private:
        bool                    m_alpha_enable; ///< the alpha enable flag.
        bool                    m_visible; ///< the visible of this layer.
        uint32_t                m_id; ///< I defined layer system as a state machine.
        float                   m_alpha_rate;
        REF(QGraphicsScene)     m_scene;
    };

    class p_graphic_layer_stack {
    public:



    private:
        REF(p_graphic_layer)                m_current_layer; ///< record the current choosed layer
        std::vector<REF(p_graphic_layer)>   m_stack;
    };

} //! namespace painters

#endif // P_LAYER_MANAGE_H
