#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Signal and slots connection
    connect(&m_brush_inspector, SIGNAL(signal_pen_changed(QPen&)), &m_default_scene, SLOT(slots_brush_pen_changed(QPen&)));
    connect(&m_default_scene, SIGNAL(signal_update_layer_tree()), this, SLOT(slots_tree_node_update()));

    ///< the main draw window settings.
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->insertTab(0, gv, "Main drawing pad");
    gv->setScene(&m_default_scene);
    // ui->graphicsView_main->setScene(&m_default_scene);

    // init brush inspector
    m_brush_inspector.set_r_slider(ui->horizontalSlider_brush_r);
    m_brush_inspector.set_g_slider(ui->horizontalSlider_brush_g);
    m_brush_inspector.set_b_slider(ui->horizontalSlider_brush_b);
    m_brush_inspector.set_a_slider(ui->horizontalSlider_brush_a);
    m_brush_inspector.set_choose_button(ui->pushButton_brush_color);
    m_brush_inspector.set_brush_type_combobox(ui->comboBox_brush_type);
    m_brush_inspector.set_graphic_view(ui->graphicsView_brush);
    m_brush_inspector.set_brush_thickness_spinbox(ui->doubleSpinBox_brush_thickness);
    m_brush_inspector.__init__();

    // init other inspector TODO.
}

void MainWindow::slots_tree_node_update() {
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);
    painters::p_graphic_layer_stack &tmp_stack = m_default_scene.m_layer_stack;
    for (auto &layer: tmp_stack) {
        QTreeWidgetItem *top_item = new QTreeWidgetItem(ui->treeWidget);

        top_item->setText(0, layer->get_name().c_str());
        ui->treeWidget->addTopLevelItem(top_item);

        std::map<std::string, REF(QGraphicsItem)> &tmp_nodes = layer->get_nodes();

        for (auto &node: tmp_nodes) {
            QStringList c;
            c << node.first.c_str();
            QTreeWidgetItem *child_item = new QTreeWidgetItem(top_item, c);
        }
    }
    ui->treeWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
