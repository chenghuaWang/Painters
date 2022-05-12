#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRect>
#include <QTreeWidget>
#include "P_GraphicViewport.h"
#include "P_CanvasWidget.h"
#include "core_base.h"
#include "p_inspector.h"
#include "P_layer_manage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slots_tree_node_update();
    void slots_export_files(bool enable);

    void slots_change_table_to_pen(bool enable);
    void slots_change_table_to_shape(bool enable);
    void slots_change_table_to_image(bool enable);

    void slots_change_shape_to_rect();
    void slots_change_shape_to_circle();

    void slots_change_shape_type(const QString& _a);

private:
    painters::p_brush_inspector     m_brush_inspector;
    painters::p_layer_inspector     m_layer_inspector;
    painters::p_shape_inspector     m_shape_inspector;
    painters::p_image_inspector     m_image_inspector;

private:
    painters::p_graphic_view *gv = new painters::p_graphic_view();
    Ui::MainWindow  *ui;
    painters::p_canvas  m_default_scene;
};
#endif // MAINWINDOW_H
