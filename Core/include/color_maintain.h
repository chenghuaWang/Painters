/*!
 * @author chenghua.wang
 * @brief A small class for Front layer UI to maintain
 * the color infomation.
 */

#ifndef COLOR_MAINTAIN_H
#define COLOR_MAINTAIN_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

#include "core_base.h"

namespace painters {

    class p_ui_color_maintain {
    public:
        p_ui_color_maintain(const std::string &name= "color_maintain"):
            m_alpha(false),
            m_color(0, 0, 0, 255),
            m_debug_name(name) {
        }

    public:
        bool                is_alpha() { return m_alpha; }
        QColor              &get_color() { return m_color; }
        const QColor        &get_color_const() { return m_color; }
        std::string         &get_name() { return m_debug_name; }
        const std::string   &get_name_const() { return m_debug_name; }
        void                set_alpha(bool enable);
        void                set_color(const QColor &_a) { m_color = _a; }
        void                set_name(const std::string &_a) { m_debug_name = _a; }

    private:
        bool            m_alpha;
        QColor          m_color;
        std::string     m_debug_name;
    };

}

#endif // COLOR_MAINTAIN_H
