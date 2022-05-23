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
#include "file_system.h"
#include <QGraphicsItem>
#include <QGraphicsScene>

namespace painters {

    /*!
     * @note Don't use shared ptr for Qt self managed ptr !!!
     */
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

        void add_node(const std::string &name, QGraphicsItem* item_ptr, const std::string &father_type ) {
            m_nodes[name] = item_ptr;
            m_type_translate_node[item_ptr] = father_type;
        }

        void delete_node(QGraphicsItem* item_ptr) {
            std::map<std::string, QGraphicsItem*>::iterator iter = m_nodes.begin();
            while(iter != m_nodes.end() && iter->second != item_ptr) {
                iter++;
            }
            if (iter != m_nodes.end()) {
                m_nodes.erase(iter);
            }
        }

        bool has_node(QGraphicsItem* item_ptr) {
            std::map<std::string, QGraphicsItem*>::iterator iter = m_nodes.begin();
            while(iter != m_nodes.end() && iter->second != item_ptr) {
                iter++;
            }
            if (iter != m_nodes.end()) {
                return true;
            }
            return false;
        }

        void rename(const std::string &name) { m_layer_name = name; }
        const std::string &get_name() { return m_layer_name; }

        std::map<std::string, QGraphicsItem*> &get_nodes() { return m_nodes; }

        int node_nums() { return (int)m_nodes.size(); }

    public:
        uint32_t                                    m_w = 1024, m_h = 720;
        bool                                        m_locked = false;

    private:
        bool                                        m_visible;
        int32_t                                     m_layer_zbuffer;
        std::string                                 m_layer_name;

    public:
        std::map<std::string, QGraphicsItem*>       m_nodes;
        std::map<QGraphicsItem*, std::string>       m_type_translate_node;
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

        void rewrite_to_file(const std::string &file_path) {
            p_project_to_json writer;
            /* prepare json_phase_payload payload */
            json_phase_payload _a;
            _a.m_layer_nums = (int)get_size();

            writer._a = _a;
            for (int i = 0; i < _a.m_layer_nums; ++i) {
                REF(p_graphic_layer) tmp_layer = m_layer_stack[i];
                /* prepare the layer payload */
                json_layer_payload prepare_component;
                prepare_component.m_layer_name = m_layer_stack[i]->get_name();
                prepare_component.m_component_nums = tmp_layer->node_nums();
                prepare_component.height = tmp_layer->m_h;
                prepare_component.width = tmp_layer->m_w;

                for (auto &item: tmp_layer->m_nodes) {
                    if (tmp_layer->m_type_translate_node[item.second] == "p_brush_component") {
                        prepare_component.m_path_item[(QGraphicsPathItem*)item.second] = item.first;
                    }
                    else if (tmp_layer->m_type_translate_node[item.second] == "p_rect_component"){
                        prepare_component.m_rect_item[(QGraphicsRectItem*)item.second] = item.first;
                    }
                    else if (tmp_layer->m_type_translate_node[item.second] == "p_circle_component"){
                        prepare_component.m_circle_item[(QGraphicsEllipseItem*)item.second] = item.first;
                    }
                    else if (tmp_layer->m_type_translate_node[item.second] == "p_image_component"){
                        prepare_component.m_pixmap_item[(QGraphicsPixmapItem*)item.second] = item.first;
                    }
                    else {
//                        qDebug() << tmp_layer->m_type_translate_node[item.second].c_str();
                    }
                }
                writer.m_data.push_back(prepare_component);
            }
            writer.write_to(file_path);
        }

    public:
        ///< Make this class works like a vector;
        std::vector<REF(p_graphic_layer)>::iterator begin() { return m_layer_stack.begin(); }
        std::vector<REF(p_graphic_layer)>::iterator end() { return m_layer_stack.end(); }
        const std::vector<REF(p_graphic_layer)>::const_iterator begin_const() { return m_layer_stack.begin(); }
        const std::vector<REF(p_graphic_layer)>::const_iterator end_const() { return m_layer_stack.end(); }

    public:
        std::vector<REF(p_graphic_layer)>     m_layer_stack;
    };

} //! namespace painters

#endif // P_LAYER_MANAGE_H
