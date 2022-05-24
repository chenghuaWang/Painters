#include "file_system.h"
#include <QFile>
#include <QTextStream>
#include <QBuffer>

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
        tmp_payload.height = _layer.value("height").toInt();
        tmp_payload.width = _layer.value("width").toInt();

        int _tmp_component_nums = tmp_payload.m_component_nums;
        for (int j = 0; j < _tmp_component_nums; ++j) {
            QJsonObject _component  = _layer.value(QString::fromStdString("component__" + std::to_string(j))).toObject();
            std::string _component_type = _component.value("type").toString().toStdString();
            std::string _component_name = _component.value("name").toString().toStdString();

            if (_component_type == CLASS_TYPE_STR(QGraphicsPathItem)) {
                QGraphicsPathItem *item = new QGraphicsPathItem();

                QPainterPath path_tmp;

                /*       Read lists of points.       */
                QJsonArray point_array_x = _component.value("position_x").toArray();
                QJsonArray point_array_y = _component.value("position_y").toArray();
                size_t size = point_array_x.size();

                path_tmp.moveTo(point_array_x.at(0).toDouble(),
                                point_array_y.at(0).toDouble());

                for (size_t s = 0; s < size ; ++s) {
                    qreal x = point_array_x.at(s).toDouble();
                    qreal y = point_array_y.at(s).toDouble();
                    path_tmp.lineTo(x, y);
                }
                item->setPath(path_tmp);
                /*       Read pen color       */
                QJsonArray pen_color = _component.value("pen_color").toArray();
                QColor  pen_c(pen_color.at(0).toInt(),
                        pen_color.at(1).toInt(),
                        pen_color.at(2).toInt(),
                        pen_color.at(3).toInt());
                QPen pen;
                pen.setStyle(Qt::SolidLine);
                pen.setWidthF(_component.value("pen_width").toDouble());
                pen.setColor(pen_c);
                /*      Read the  pen type  TODO*/
                item->setPen(pen);

                /*      Read the position of this item  */
                QJsonArray pos_tmp = _component.value("position").toArray();
                QPointF    pos(pos_tmp.at(0).toDouble(), pos_tmp.at(1).toDouble());
                item->setPos(pos);

                /*      Read the transformation of this item */
                QJsonArray transform_tmp = _component.value("transform").toArray();
                QTransform transform;
                transform.setMatrix(transform_tmp.at(0).toDouble(),
                                    transform_tmp.at(1).toDouble(),
                                    transform_tmp.at(2).toDouble(),
                                    transform_tmp.at(3).toDouble(),
                                    transform_tmp.at(4).toDouble(),
                                    transform_tmp.at(5).toDouble(),
                                    transform_tmp.at(6).toDouble(),
                                    transform_tmp.at(7).toDouble(),
                                    transform_tmp.at(8).toDouble());
                item->setTransform(transform);
                item->setVisible(true);

                item->setScale(_component.value("scale").toDouble());
                item->setRotation(_component.value("rotate").toDouble());

                tmp_payload.m_path_item[item] = _component_name;
            }
            else if(_component_type == CLASS_TYPE_STR(QGraphicsRectItem)) {
                QGraphicsRectItem *item = new QGraphicsRectItem();

                /*       Read the base rect item info       */
                QJsonArray tmp_base_points = _component.value("rect_base_pos").toArray();
                QRectF base_pointd = QRectF(tmp_base_points.at(0).toDouble(),
                                            tmp_base_points.at(1).toDouble(),
                                            tmp_base_points.at(2).toDouble(),
                                            tmp_base_points.at(3).toDouble());
                item->setRect(base_pointd);

                /*       Read pen color       */
                QJsonArray pen_color = _component.value("pen_color").toArray();
                QColor  pen_c(pen_color.at(0).toInt(),
                        pen_color.at(1).toInt(),
                        pen_color.at(2).toInt(),
                        pen_color.at(3).toInt());
                QPen pen;
                pen.setStyle(Qt::SolidLine);
                pen.setWidthF(_component.value("pen_width").toDouble());
                pen.setColor(pen_c);
                item->setPen(pen);

                /*      Read the brush      */
                QJsonArray brush_color = _component.value("brush_color").toArray();
                QColor  brush_c(brush_color.at(0).toInt(),
                                brush_color.at(1).toInt(),
                                brush_color.at(2).toInt(),
                                brush_color.at(3).toInt());
                QBrush brush;
                brush.setColor(brush_c);
                brush.setStyle(Qt::SolidPattern);
                item->setBrush(brush);

                /*      Read the position of this item  */
                QJsonArray pos_tmp = _component.value("position").toArray();
                QPointF    pos(pos_tmp.at(0).toDouble(), pos_tmp.at(1).toDouble());
                item->setPos(pos);

                /*      Read the transformation of this item */
                QJsonArray transform_tmp = _component.value("transform").toArray();
                QTransform transform;
                transform.setMatrix(transform_tmp.at(0).toDouble(),
                                    transform_tmp.at(1).toDouble(),
                                    transform_tmp.at(2).toDouble(),
                                    transform_tmp.at(3).toDouble(),
                                    transform_tmp.at(4).toDouble(),
                                    transform_tmp.at(5).toDouble(),
                                    transform_tmp.at(6).toDouble(),
                                    transform_tmp.at(7).toDouble(),
                                    transform_tmp.at(8).toDouble());
                item->setTransform(transform);
                item->setVisible(true);

                item->setScale(_component.value("scale").toDouble());
                item->setRotation(_component.value("rotate").toDouble());

                tmp_payload.m_rect_item[item] = _component_name;
            }
            else if(_component_type == CLASS_TYPE_STR(QGraphicsEllipseItem)) {
                QGraphicsEllipseItem *item = new QGraphicsEllipseItem();

                /*       Read the base rect item info       */
                QJsonArray tmp_base_points = _component.value("rect_base_pos").toArray();
                QRectF base_pointd = QRectF(tmp_base_points.at(0).toDouble(),
                                            tmp_base_points.at(1).toDouble(),
                                            tmp_base_points.at(2).toDouble(),
                                            tmp_base_points.at(3).toDouble());
                item->setRect(base_pointd);

                /*       Read pen color       */
                QJsonArray pen_color = _component.value("pen_color").toArray();
                QColor  pen_c(pen_color.at(0).toInt(),
                        pen_color.at(1).toInt(),
                        pen_color.at(2).toInt(),
                        pen_color.at(3).toInt());
                QPen pen;
                pen.setStyle(Qt::SolidLine);
                pen.setWidthF(_component.value("pen_width").toDouble());
                pen.setColor(pen_c);
                item->setPen(pen);

                /*      Read the brush      */
                QJsonArray brush_color = _component.value("brush_color").toArray();
                QColor  brush_c(brush_color.at(0).toInt(),
                                brush_color.at(1).toInt(),
                                brush_color.at(2).toInt(),
                                brush_color.at(3).toInt());
                QBrush brush;
                brush.setColor(brush_c);
                brush.setStyle(Qt::SolidPattern);
                item->setBrush(brush);

                /*      Read the position of this item  */
                QJsonArray pos_tmp = _component.value("position").toArray();
                QPointF    pos(pos_tmp.at(0).toDouble(), pos_tmp.at(1).toDouble());
                item->setPos(pos);

                /*      Read the transformation of this item */
                QJsonArray transform_tmp = _component.value("transform").toArray();
                QTransform transform;
                transform.setMatrix(transform_tmp.at(0).toDouble(),
                                    transform_tmp.at(1).toDouble(),
                                    transform_tmp.at(2).toDouble(),
                                    transform_tmp.at(3).toDouble(),
                                    transform_tmp.at(4).toDouble(),
                                    transform_tmp.at(5).toDouble(),
                                    transform_tmp.at(6).toDouble(),
                                    transform_tmp.at(7).toDouble(),
                                    transform_tmp.at(8).toDouble());
                item->setTransform(transform);
                item->setVisible(true);

                item->setScale(_component.value("scale").toDouble());
                item->setRotation(_component.value("rotate").toDouble());

                tmp_payload.m_circle_item[item] = _component_name;
            }
            else if(_component_type == CLASS_TYPE_STR(QGraphicsPixmapItem)) {
                QGraphicsPixmapItem *item = new QGraphicsPixmapItem();

                /*      Read the image from base64       */
                QString tmp_data = _component.value("image_data").toString();
                QPixmap tmp_pixmap;
                tmp_pixmap.loadFromData(QByteArray::fromBase64(tmp_data.toLocal8Bit()));
                item->setPixmap(tmp_pixmap);

                /*      Read the position of this item  */
                QJsonArray pos_tmp = _component.value("position").toArray();
                QPointF    pos(pos_tmp.at(0).toDouble(), pos_tmp.at(1).toDouble());
                item->setPos(pos);

                /*      Read the transformation of this item */
                QJsonArray transform_tmp = _component.value("transform").toArray();
                QTransform transform;
                transform.setMatrix(transform_tmp.at(0).toDouble(),
                                    transform_tmp.at(1).toDouble(),
                                    transform_tmp.at(2).toDouble(),
                                    transform_tmp.at(3).toDouble(),
                                    transform_tmp.at(4).toDouble(),
                                    transform_tmp.at(5).toDouble(),
                                    transform_tmp.at(6).toDouble(),
                                    transform_tmp.at(7).toDouble(),
                                    transform_tmp.at(8).toDouble());
                item->setTransform(transform);
                item->setVisible(true);

                item->setScale(_component.value("scale").toDouble());
                item->setRotation(_component.value("rotate").toDouble());

                tmp_payload.m_pixmap_item[item] = _component_name;
            }
        }

        m_layers_payload.push_back(tmp_payload);
    }

    return false;
}
}

namespace painters {
void p_project_to_json::write_to(const std::string &file_path) {
    QJsonDocument   doc; ///< the abstract of json document;
    QJsonObject rootObj;
    rootObj.insert("magic_number", QJsonValue(MAGIC_NUMBER));
    rootObj.insert("layer_nums", QJsonValue(_a.m_layer_nums));

    for (int i = 0; i < _a.m_layer_nums; ++i) {
        QJsonObject layer_obj;
        layer_obj.insert("name", QJsonValue(QString::fromStdString(m_data[i].m_layer_name)));
        layer_obj.insert("width", QJsonValue(m_data[i].width));
        layer_obj.insert("height", QJsonValue(m_data[i].height));
        layer_obj.insert("component_nums", QJsonValue(m_data[i].m_component_nums));
        int component_cnt = 0;

        for (auto &item:m_data[i].m_path_item) {
            QJsonObject component_obj;
            component_obj.insert("type", QJsonValue(CLASS_TYPE_STR(QGraphicsPathItem)));
            component_obj.insert("name", QJsonValue(QString::fromStdString(item.second)));
            QJsonArray  tmp_x, tmp_y;
            int points_lists_size = item.first->path().elementCount();
            for (int cnt = 0; cnt < points_lists_size; ++cnt) {
                tmp_x.append(QJsonValue(item.first->path().elementAt(cnt).x));
                tmp_y.append(QJsonValue(item.first->path().elementAt(cnt).y));
            }
            component_obj.insert("position_x", tmp_x);
            component_obj.insert("position_y", tmp_y);

            QColor tmp_color = item.first->pen().color();
            QJsonArray color;
            color.append(QJsonValue(tmp_color.red()));
            color.append(QJsonValue(tmp_color.green()));
            color.append(QJsonValue(tmp_color.blue()));
            color.append(QJsonValue(tmp_color.alpha()));

            component_obj.insert("pen_color", color);

            QPointF pos_tmp = item.first->pos();
            QJsonArray pos;
            pos.append(QJsonValue(pos_tmp.x()));
            pos.append(QJsonValue(pos_tmp.y()));

            component_obj.insert("position", pos);

            QTransform transform_obj = item.first->transform();
            QJsonArray transform_tmp;
            transform_tmp.append(QJsonValue(transform_obj.m11()));
            transform_tmp.append(QJsonValue(transform_obj.m12()));
            transform_tmp.append(QJsonValue(transform_obj.m13()));
            transform_tmp.append(QJsonValue(transform_obj.m21()));
            transform_tmp.append(QJsonValue(transform_obj.m22()));
            transform_tmp.append(QJsonValue(transform_obj.m23()));
            transform_tmp.append(QJsonValue(transform_obj.m31()));
            transform_tmp.append(QJsonValue(transform_obj.m32()));
            transform_tmp.append(QJsonValue(transform_obj.m33()));

            component_obj.insert("transform", transform_tmp);
            component_obj.insert("pen_width", item.first->pen().widthF());

            component_obj.insert("scale", QJsonValue(item.first->scale()));
            component_obj.insert("rotate", QJsonValue(item.first->rotation()));

            layer_obj.insert(QString::fromStdString("component__" + std::to_string(component_cnt ++)), component_obj);
        }

        for (auto &item:m_data[i].m_rect_item) {
            QJsonObject component_obj;
            component_obj.insert("type", QJsonValue(CLASS_TYPE_STR(QGraphicsRectItem)));
            component_obj.insert("name", QJsonValue(QString::fromStdString(item.second)));

            QJsonArray rect_base_point;
            QRectF tmp_rect_base_point = item.first->rect();
            rect_base_point.append(QJsonValue(tmp_rect_base_point.x()));
            rect_base_point.append(QJsonValue(tmp_rect_base_point.y()));
            rect_base_point.append(QJsonValue(tmp_rect_base_point.width()));
            rect_base_point.append(QJsonValue(tmp_rect_base_point.height()));
            component_obj.insert("rect_base_pos", rect_base_point);

            /* TODO Add Brush. */
            QBrush tmp_brush = item.first->brush();
            QJsonArray brush_color;
            brush_color.append(QJsonValue(tmp_brush.color().red()));
            brush_color.append(QJsonValue(tmp_brush.color().green()));
            brush_color.append(QJsonValue(tmp_brush.color().blue()));
            brush_color.append(QJsonValue(tmp_brush.color().alpha()));
            component_obj.insert("brush_color", brush_color);

            QColor tmp_color = item.first->pen().color();
            QJsonArray color;
            color.append(QJsonValue(tmp_color.red()));
            color.append(QJsonValue(tmp_color.green()));
            color.append(QJsonValue(tmp_color.blue()));
            color.append(QJsonValue(tmp_color.alpha()));

            component_obj.insert("pen_color", color);

            QPointF pos_tmp = item.first->pos();
            QJsonArray pos;
            pos.append(QJsonValue(pos_tmp.x()));
            pos.append(QJsonValue(pos_tmp.y()));

            component_obj.insert("position", pos);

            QTransform transform_obj = item.first->transform();
            QJsonArray transform_tmp;
            transform_tmp.append(QJsonValue(transform_obj.m11()));
            transform_tmp.append(QJsonValue(transform_obj.m12()));
            transform_tmp.append(QJsonValue(transform_obj.m13()));
            transform_tmp.append(QJsonValue(transform_obj.m21()));
            transform_tmp.append(QJsonValue(transform_obj.m22()));
            transform_tmp.append(QJsonValue(transform_obj.m23()));
            transform_tmp.append(QJsonValue(transform_obj.m31()));
            transform_tmp.append(QJsonValue(transform_obj.m32()));
            transform_tmp.append(QJsonValue(transform_obj.m33()));

            component_obj.insert("transform", transform_tmp);
            component_obj.insert("pen_width", item.first->pen().widthF());

            component_obj.insert("scale", QJsonValue(item.first->scale()));
            component_obj.insert("rotate", QJsonValue(item.first->rotation()));

            layer_obj.insert(QString::fromStdString("component__" + std::to_string(component_cnt ++)), component_obj);
        }

        for (auto &item:m_data[i].m_circle_item) {
            QJsonObject component_obj;
            component_obj.insert("type", QJsonValue(CLASS_TYPE_STR(QGraphicsEllipseItem)));
            component_obj.insert("name", QJsonValue(QString::fromStdString(item.second)));

            QJsonArray rect_base_point;
            QRectF tmp_rect_base_point = item.first->rect();
            rect_base_point.append(QJsonValue(tmp_rect_base_point.x()));
            rect_base_point.append(QJsonValue(tmp_rect_base_point.y()));
            rect_base_point.append(QJsonValue(tmp_rect_base_point.width()));
            rect_base_point.append(QJsonValue(tmp_rect_base_point.height()));
            component_obj.insert("rect_base_pos", rect_base_point);

            /* TODO Add Brush. */
            QBrush tmp_brush = item.first->brush();
            QJsonArray brush_color;
            brush_color.append(QJsonValue(tmp_brush.color().red()));
            brush_color.append(QJsonValue(tmp_brush.color().green()));
            brush_color.append(QJsonValue(tmp_brush.color().blue()));
            brush_color.append(QJsonValue(tmp_brush.color().alpha()));
            component_obj.insert("brush_color", brush_color);

            QColor tmp_color = item.first->pen().color();
            QJsonArray color;
            color.append(QJsonValue(tmp_color.red()));
            color.append(QJsonValue(tmp_color.green()));
            color.append(QJsonValue(tmp_color.blue()));
            color.append(QJsonValue(tmp_color.alpha()));

            component_obj.insert("pen_color", color);

            QPointF pos_tmp = item.first->pos();
            QJsonArray pos;
            pos.append(QJsonValue(pos_tmp.x()));
            pos.append(QJsonValue(pos_tmp.y()));

            component_obj.insert("position", pos);

            QTransform transform_obj = item.first->transform();
            QJsonArray transform_tmp;
            transform_tmp.append(QJsonValue(transform_obj.m11()));
            transform_tmp.append(QJsonValue(transform_obj.m12()));
            transform_tmp.append(QJsonValue(transform_obj.m13()));
            transform_tmp.append(QJsonValue(transform_obj.m21()));
            transform_tmp.append(QJsonValue(transform_obj.m22()));
            transform_tmp.append(QJsonValue(transform_obj.m23()));
            transform_tmp.append(QJsonValue(transform_obj.m31()));
            transform_tmp.append(QJsonValue(transform_obj.m32()));
            transform_tmp.append(QJsonValue(transform_obj.m33()));

            component_obj.insert("transform", transform_tmp);
            component_obj.insert("pen_width", item.first->pen().widthF());

            component_obj.insert("scale", QJsonValue(item.first->scale()));
            component_obj.insert("rotate", QJsonValue(item.first->rotation()));

            layer_obj.insert(QString::fromStdString("component__" + std::to_string(component_cnt ++)), component_obj);
        }

        for (auto &item:m_data[i].m_pixmap_item) {
            QJsonObject component_obj;
            component_obj.insert("type", QJsonValue(CLASS_TYPE_STR(QGraphicsPixmapItem)));
            component_obj.insert("name", QJsonValue(QString::fromStdString(item.second)));

            /*          preapre image to base64     */
            QImage tmp_image = item.first->pixmap().toImage();
            QByteArray ba;
            QBuffer buf(&ba);
            tmp_image.save(&buf, "png");
            component_obj.insert("image_data", ba.toBase64().toStdString().c_str());

            QPointF pos_tmp = item.first->pos();
            QJsonArray pos;
            pos.append(QJsonValue(pos_tmp.x()));
            pos.append(QJsonValue(pos_tmp.y()));

            component_obj.insert("position", pos);

            QTransform transform_obj = item.first->transform();
            QJsonArray transform_tmp;
            transform_tmp.append(QJsonValue(transform_obj.m11()));
            transform_tmp.append(QJsonValue(transform_obj.m12()));
            transform_tmp.append(QJsonValue(transform_obj.m13()));
            transform_tmp.append(QJsonValue(transform_obj.m21()));
            transform_tmp.append(QJsonValue(transform_obj.m22()));
            transform_tmp.append(QJsonValue(transform_obj.m23()));
            transform_tmp.append(QJsonValue(transform_obj.m31()));
            transform_tmp.append(QJsonValue(transform_obj.m32()));
            transform_tmp.append(QJsonValue(transform_obj.m33()));

            component_obj.insert("transform", transform_tmp);

            component_obj.insert("scale", QJsonValue(item.first->scale()));
            component_obj.insert("rotate", QJsonValue(item.first->rotation()));

            layer_obj.insert(QString::fromStdString("component__" + std::to_string(component_cnt ++)), component_obj);
        }

        rootObj.insert(QString::fromStdString("layer__" + std::to_string(i)), layer_obj);
    }
    doc.setObject(rootObj);

    QFile file(file_path.c_str());
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }
    QTextStream stream(&file);
    stream.setAutoDetectUnicode(true);
    QByteArray byte_data = doc.toJson(QJsonDocument::Compact);
    QString json_str(byte_data);
    stream << json_str;
    file.close();
}
}
