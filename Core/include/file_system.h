/*!
 * @author  chenghua.wang
 * @brief   Manage the data of this project.
 * I use json file to record all components in a scene.
 * ans use zip method to package it.
 *
 * @details Create A json first. when item changed. Just
 * write to this json file. In that case, we can avoid
 * QDataStram or using boost Serialization library.
 */

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "core_base.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>

#define CLASS_TYPE_STR(s) _s_(s)
#define _s_(s) #s

#define MAGIC_NUMBER 42

namespace painters {

    struct json_layer_payload {
        int m_component_nums;
        std::string m_layer_name;
        std::map<QGraphicsPathItem*, std::string> m_path_item;
        std::map<QGraphicsRectItem*, std::string> m_rect_item;
        std::map<QGraphicsEllipseItem*, std::string> m_circle_item;
        std::map<QGraphicsPixmapItem*, std::string> m_pixmap_item;
    };

    struct json_phase_payload {
        int   m_layer_nums;

    };

    class p_project_phaser {
    public:
        bool phase(const std::string &rhs);

        json_phase_payload data() const { return _a;}

    private:
        json_phase_payload _a;
        std::vector<json_layer_payload> m_layers_payload;
    };

}

#endif // FILE_SYSTEM_H
