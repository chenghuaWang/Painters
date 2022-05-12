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

    class p_graphic_layer: QWidget {
        Q_OBJECT
    public:
        p_graphic_layer(const std::string &name): //uint32_t w, uint32_t h
            m_visible(true),
            m_layer_name(name){
            m_layer_zbuffer = -1;
        };

        int32_t get_zbuffer() { return m_layer_zbuffer; }
        void set_zbuffer(int32_t _a) { m_layer_zbuffer = _a; }

        bool get_visible() { return m_visible; }
        void set_visible(bool enable) { m_visible = enable; }

        uint32_t get_w() { return m_w; }
        uint32_t get_h() { return m_h; }

        void add_node(const std::string &name, QGraphicsItem* item_ptr ) {
            m_nodes[name] = REF(QGraphicsItem)(item_ptr);
        }

        void rename(const std::string &name) { m_layer_name = name; }
        const std::string &get_name() { return m_layer_name; }

        std::map<std::string, REF(QGraphicsItem)> &get_nodes() { return m_nodes; }

    public:
        uint32_t                                    m_w, m_h;
        bool                                        m_locked = false;

    private:
        bool                                        m_visible;
        int32_t                                     m_layer_zbuffer;
        std::string                                 m_layer_name;
        std::map<std::string, REF(QGraphicsItem)>   m_nodes;
    };

    class p_graphic_layer_stack {
    public:
        p_graphic_layer_stack() {}

        void push_layer(REF(p_graphic_layer) &_a) {
            m_layer_stack.push_back(_a);
        }

        void pop_layer(REF(p_graphic_layer) &_a) {
            std::vector<REF(p_graphic_layer)>::iterator result = find(m_layer_stack.begin( ), m_layer_stack.end( ), _a);
            if (result != end()) {
                m_layer_stack.erase(result);
            }
        }

        size_t get_size() { return m_layer_stack.size(); }

    public:
        ///< Make this class works like a vector;
        std::vector<REF(p_graphic_layer)>::iterator begin() { return m_layer_stack.begin(); }
        std::vector<REF(p_graphic_layer)>::iterator end() { return m_layer_stack.end(); }
        const std::vector<REF(p_graphic_layer)>::const_iterator begin_const() { return m_layer_stack.begin(); }
        const std::vector<REF(p_graphic_layer)>::const_iterator end_const() { return m_layer_stack.end(); }

    private:
        std::vector<REF(p_graphic_layer)>     m_layer_stack;
    };

} //! namespace painters

#endif // P_LAYER_MANAGE_H
