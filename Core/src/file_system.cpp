#include "file_system.h"
#include <QFile>
#include <QTextStream>

namespace painters {

bool p_project_phaser::phase(const std::string &rhs) {
    QFile file(rhs.c_str());
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    QTextStream stream(&file);
    stream.setAutoDetectUnicode(true);
    QString str = stream.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        return false;
    }

    QJsonObject rootObj = doc.object();

    if (!(rootObj.value("magic_number").toInt() == MAGIC_NUMBER)) {
        return false;
    }

    _a.m_layer_nums = rootObj.value("layer_nums").toInt();

    std::string virtual_layer_name = "layer__";

    for (int i = 0; i < _a.m_layer_nums; ++i) {
        QJsonObject _layer = rootObj.value(QString::fromStdString(virtual_layer_name + std::to_string(i))).toObject();
        json_layer_payload tmp_payload;

        tmp_payload.m_layer_name = _layer.value("name").toString().toStdString();
        tmp_payload.m_component_nums = _layer.value("component_nums").toInt();

        int _tmp_component_nums = tmp_payload.m_component_nums;
        for (int j = 0; j < _tmp_component_nums; ++j) {
            QJsonObject _component  = _layer.value(QString::fromStdString("component__" + std::to_string(j))).toObject();
            std::string _component_type = _component.value("type").toString().toStdString();
            std::string _component_name = _component.value("name").toString().toStdString();

            if (_component_type == CLASS_TYPE_STR(QGraphicsPathItem)) {
                QGraphicsPathItem *item = new QGraphicsPathItem();

                // TODO read [position, point-array, transform-matrix, Qpen, QBrush]

                tmp_payload.m_path_item[item] = _component_name;
            }
            else if(_component_type == CLASS_TYPE_STR(QGraphicsRectItem)) {

            }
            else if(_component_type == CLASS_TYPE_STR(QGraphicsEllipseItem)) {

            }
            else if(_component_type == CLASS_TYPE_STR(QGraphicsPixmapItem)) {

            }
        }

        m_layers_payload.push_back(tmp_payload);
    }

    return false;
}

}
