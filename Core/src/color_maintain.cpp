#include "color_maintain.h"

namespace painters {

void p_ui_color_maintain::set_alpha(bool enable) {
    if (enable) {
        m_color.setAlpha(128);
    }
    else {
        m_color.setAlpha(255);
    }
}

}
