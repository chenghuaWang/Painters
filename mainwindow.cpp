#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set reference line
    ui->comboBox_reference->addItem("None");
    ui->comboBox_reference->addItem("default");

    // set icon
    ui->toolButton_pen->setIcon(QIcon(":/icon/brush_paint_icon_193942.ico"));

    // Signal and slots connection
    connect(&m_brush_inspector, SIGNAL(signal_pen_changed(QPen&)), &m_default_scene, SLOT(slots_brush_pen_changed(QPen&)));
    connect(&m_default_scene, SIGNAL(signal_update_layer_tree()), this, SLOT(slots_tree_node_update()));
    connect(ui->comboBox_reference, SIGNAL(currentTextChanged(const QString&)), &m_default_scene, SLOT(slots_process_reference(const QString&)));
    connect(ui->checkBox_layer_lock, SIGNAL(clicked(bool)), &m_default_scene, SLOT(slots_lock_cur_layer(bool)));
    connect(ui->actionExport_Files, SIGNAL(triggered(bool)), this, SLOT(slots_export_files(bool)));

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
    m_brush_inspector.set_brush_cap_combobox(ui->comboBox_pen_cap_style);
    m_brush_inspector.set_brush_join_combobox(ui->comboBox_pen_join_style);
    m_brush_inspector.__init__();

    // init layer inspector.
    m_layer_inspector.set_alpha_enable_checkbox(ui->checkBox_layer_alpha);
    m_layer_inspector.set_lock_checkbox(ui->checkBox_layer_lock);
    m_layer_inspector.set_name_editor(ui->lineEdit_layer_name);
    m_layer_inspector.set_size_editor(ui->lineEdit_layer_size);
    m_layer_inspector.set_alpha_spinbox(ui->doubleSpinBox_alpha_value);
    m_layer_inspector.__init__();

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

    m_layer_inspector.slots_update_layer_widget(m_default_scene.m_cur_choosed_layer);
}

void MainWindow::slots_export_files(bool enable) {
    QString dir_path = QFileDialog::getExistingDirectory(this, "choose directory", "C:/Users/Administrator/Desktop");
    QString default_name = "foo.png";

    m_default_scene.m_rectItem->setVisible(false);
    if (m_default_scene.m_reference_line) m_default_scene.m_reference_line->setVisible(false);
    if (m_default_scene.m_reference_line_v) m_default_scene.m_reference_line_v->setVisible(false);

    QImage image(gv->scene()->width(), gv->scene()->height(), QImage::Format_ARGB32);
    QString pngName = dir_path + "/" + default_name;
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    gv->scene()->render(&painter);
    bool saveSuccess =  image.save(pngName);

    m_default_scene.m_rectItem->setVisible(true);
    if (m_default_scene.m_reference_line) m_default_scene.m_reference_line->setVisible(true);
    if (m_default_scene.m_reference_line_v) m_default_scene.m_reference_line_v->setVisible(true);

    Q_ASSERT(saveSuccess == true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gv;
}
