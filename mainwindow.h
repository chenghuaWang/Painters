#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRect>
#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "P_GraphicViewport.h"
#include "P_CanvasWidget.h"
#include "core_base.h"
#include "file_system.h"
#include "p_inspector.h"
#include "P_layer_manage.h"

#include "log_system.h"

#include "about_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void signal_send_action_delete_layer_from_treewidget(QTreeWidgetItem*);
    void signal_send_action_delete_component_from_treewidget(QTreeWidgetItem*);

public slots:
    void slots_tree_node_update();
    void slots_export_files(bool enable);

    void slots_change_table_to_pen(bool enable);
    void slots_change_table_to_shape(bool enable);
    void slots_change_table_to_image(bool enable);

    void slots_change_shape_to_rect();
    void slots_change_shape_to_circle();

    void slots_change_shape_type(const QString& _a);

    void slost_change_choosed_component_attribute();

    void slot_action_delete_layer(QTreeWidgetItem* rhs);
    void slot_action_delete_component(QTreeWidgetItem* rhs);

private:
    painters::p_brush_inspector     m_brush_inspector;
    painters::p_layer_inspector     m_layer_inspector;
    painters::p_shape_inspector     m_shape_inspector;
    painters::p_image_inspector     m_image_inspector;

private:
    QString                         m_saved_file_path;

private:
    ///< for tree widget to use
    QAction *m_delete_layer_action = new QAction("delete layer");
    QAction *m_delete_component_action = new QAction("delete component");
    QMenu   *m_tree_menu = new QMenu(this);
    QTreeWidgetItem *m_treewdiget_item = nullptr;

private:
    painters::p_graphic_view        *gv = new painters::p_graphic_view();
    Ui::MainWindow                  *ui;
    painters::p_canvas              m_default_scene;

    ///< create a single mode log system. only exist one time.
    p_logger                        m_logger;
    about_window                    *m_about_window;
};
#endif // MAINWINDOW_H
