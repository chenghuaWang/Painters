#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFile>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set reference line
    ui->comboBox_reference->addItem("None");
    ui->comboBox_reference->addItem("default");

    // set icon
    ui->toolButton_pen->setIcon(QIcon(":/icon/brush_paint_icon_193942.ico"));
    ui->toolButton_shape->setIcon(QIcon(":/icon/triangle-circle-shape-brand_icon-icons.com_59117.ico"));
    ui->toolButton_image->setIcon(QIcon(":/icon/icons8-image-64.png"));
    ui->toolButton_select->setIcon(QIcon(":/icon/icons8-select-64.png"));
    ui->toolButton_text->setIcon(QIcon(":/icon/icons8-text-color-96.png"));

    // Signal and slots connection
    connect(&m_brush_inspector, SIGNAL(signal_pen_changed(QPen&)), &m_default_scene, SLOT(slots_brush_pen_changed(QPen&)));
    connect(&m_default_scene, SIGNAL(signal_update_layer_tree()), this, SLOT(slots_tree_node_update()));
    connect(ui->comboBox_reference, SIGNAL(currentTextChanged(const QString&)), &m_default_scene, SLOT(slots_process_reference(const QString&)));
    connect(ui->checkBox_layer_lock, SIGNAL(clicked(bool)), &m_default_scene, SLOT(slots_lock_cur_layer(bool)));
    connect(ui->actionExport_Files, SIGNAL(triggered(bool)), this, SLOT(slots_export_files(bool)));

    connect(ui->toolButton_shape, SIGNAL(clicked(bool)), &m_default_scene, SLOT(slots_set_tool_type_shape(bool)));
    connect(ui->toolButton_pen, SIGNAL(clicked(bool)), &m_default_scene, SLOT(slots_set_tool_type_pen(bool)));
    connect(ui->toolButton_image, SIGNAL(clicked(bool)), &m_default_scene, SLOT(slots_set_tool_type_image(bool)));
    connect(ui->toolButton_select, &QPushButton::clicked, [=](){
        m_default_scene.set_tool_type(painters::tool_type::Select);
    });
    connect(ui->toolButton_text, &QPushButton::clicked, [=](){
        m_default_scene.set_tool_type(painters::tool_type::Text);
        ui->tabWidget_2->setCurrentIndex(1);
//        ui->horizontalSlider->setValue(0);
//        ui->horizontalSlider_2->setValue(0);
//        ui->horizontalSlider_3->setValue(0);
    });

    connect(ui->toolButton_shape, SIGNAL(clicked(bool)), this, SLOT(slots_change_table_to_shape(bool)));
    connect(ui->toolButton_pen, SIGNAL(clicked(bool)), this, SLOT(slots_change_table_to_pen(bool)));
    connect(ui->toolButton_image, SIGNAL(clicked(bool)), this, SLOT(slots_change_table_to_image(bool)));


    connect(ui->comboBox_shape, SIGNAL(currentTextChanged(const QString&)), this, SLOT(slots_change_shape_type(const QString&)));

    connect(&m_shape_inspector, SIGNAL(signal_pen_changed(QPen &)), &m_default_scene, SLOT(slots_shape_pen_changed(QPen&)));
    connect(&m_shape_inspector, SIGNAL(signal_brush_changed(QBrush &)), &m_default_scene, SLOT(slots_shape_brush_changed(QBrush&)));

    connect(&m_image_inspector, SIGNAL(signal_image_update(const QString &)), &m_default_scene, SLOT(slots_image_string_changed(const QString &)));

    ///< the main draw window settings.
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->insertTab(0, gv, "Main drawing pad");
    gv->setScene(&m_default_scene);
    // ui->graphicsView_main->setScene(&m_default_scene);

    // connect action
    connect(ui->actionAdd_new_layer, &QAction::triggered, [=](){
        auto name_tmp = QInputDialog::getText(this, "Create new layer", "Enter layer name");
        m_default_scene.add_layer(name_tmp.toStdString());
    });

    connect(ui->actionSave_Files, &QAction::triggered, [=](){
        if (m_saved_file_path.isEmpty()) {
            m_saved_file_path = QFileDialog::getSaveFileName(this, "Save Project", "C:", "Project File (*.projectp)");
            m_saved_file_path += ".projectp";
        }
        m_default_scene.m_layer_stack.rewrite_to_file(m_saved_file_path.toStdString());
    });

    connect(ui->actionOpen_Project, &QAction::triggered, [=](){
        QString open_file_path = QFileDialog::getOpenFileName(this, "Save Project", "C:", "Project File (*.projectp)"); //.projectp
        m_saved_file_path = open_file_path;

        for (auto &layer: m_default_scene.m_layer_stack) {
            for (auto &item: layer->m_nodes) {
                m_default_scene.removeItem(item.second);
            }
            layer->m_nodes.clear();
            layer->m_type_translate_node.clear();
        }
        m_default_scene.m_layer_stack.m_layer_stack.clear();
        this->slots_tree_node_update();

        painters::p_project_phaser reader;
        reader.phase(m_saved_file_path.toStdString());

        auto obj_base = reader.data();
        auto obj_data = reader.get_data();

        for (auto &item: obj_data) {
            m_default_scene.add_layer(item.m_layer_name);
            m_default_scene.m_cur_choosed_layer->m_h = item.height;
            m_default_scene.m_cur_choosed_layer->m_w = item.width;
            for (auto &path_item: item.m_path_item) {
                m_default_scene.m_cur_choosed_layer->add_node(path_item.second, path_item.first, "p_brush_component");
                m_default_scene.addItem(path_item.first); //path_item.first
            }
            for (auto &rect_item: item.m_rect_item) {
                m_default_scene.m_cur_choosed_layer->add_node(rect_item.second, rect_item.first, "p_rect_component");

                m_default_scene.addItem(rect_item.first); //path_item.first
            }
            for (auto &circle_item: item.m_circle_item) {
                m_default_scene.m_cur_choosed_layer->add_node(circle_item.second, circle_item.first, "p_circle_component");

                m_default_scene.addItem(circle_item.first); //path_item.first
            }
            for (auto &pixmap_item: item.m_pixmap_item) {
                m_default_scene.m_cur_choosed_layer->add_node(pixmap_item.second, pixmap_item.first, "p_circle_component");

                m_default_scene.addItem(pixmap_item.first); //path_item.first
            }
            /* TODO pixmap, rect, ellipse, other stuff */
        }
        this->slots_tree_node_update();
        m_default_scene.update();
    });

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

    // init shape inspector
    m_shape_inspector.set_brush_color_pushbutton(ui->pushButton_shape_brush_color);
    m_shape_inspector.set_pen_color_pushbutton(ui->pushButton_shape_pen_color);
    m_shape_inspector.set_thickness_spinbox(ui->doubleSpinBox_shape_thick);
    m_shape_inspector.set_shape_combobox(ui->comboBox_shape);
    m_shape_inspector.set_brush_type_combobox(ui->comboBox_brush_type_2);
    m_shape_inspector.set_view(ui->graphicsView_2);
    m_shape_inspector.__init__();

    // init image inspector
    m_image_inspector.set_view(ui->graphicsView_4);
    m_image_inspector.set_image_size_editor(ui->lineEdit_image_size);
    m_image_inspector.set_image_name_editor(ui->lineEdit_image_name);
    m_image_inspector.set_image_path_editor(ui->lineEdit_image_path);
    m_image_inspector.set_image_get_pushbutton(ui->pushButton_image_add_button);
    m_image_inspector.__init__();

    // connect font inspector
    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, [=](const QFont& _a){
        int r = ui->horizontalSlider->value();
        int g = ui->horizontalSlider_2->value();
        int b = ui->horizontalSlider_3->value();
        QFont tmp = _a;
        QPalette color;
        color.setColor(QPalette::WindowText, QColor(r, g, b));
        tmp.setPixelSize(ui->spinBox_font->value());
        m_default_scene.slots_text_font_changed(tmp);
        ui->label_11->setFont(tmp);
        ui->label_11->setPalette(color);
        ui->label_12->setFont(tmp);
        ui->label_12->setPalette(color);
        ui->label_13->setFont(tmp);
        ui->label_13->setPalette(color);
    });

    connect(ui->horizontalSlider, &QSlider::sliderMoved, [=](){
        int r = ui->horizontalSlider->value();
        int g = ui->horizontalSlider_2->value();
        int b = ui->horizontalSlider_3->value();
        QPalette color;
        color.setColor(QPalette::WindowText, QColor(r, g, b));
        ui->label_11->setPalette(color);
        ui->label_12->setPalette(color);
        ui->label_13->setPalette(color);

        m_default_scene.slots_text_color_changed(QColor(r,g,b));
    });

    connect(ui->horizontalSlider_2, &QSlider::sliderMoved, [=](){
        int r = ui->horizontalSlider->value();
        int g = ui->horizontalSlider_2->value();
        int b = ui->horizontalSlider_3->value();
        QPalette color;
        color.setColor(QPalette::WindowText, QColor(r, g, b));
        ui->label_11->setPalette(color);
        ui->label_12->setPalette(color);
        ui->label_13->setPalette(color);

        m_default_scene.slots_text_color_changed(QColor(r,g,b));
    });

    connect(ui->horizontalSlider_3, &QSlider::sliderMoved, [=](){
        int r = ui->horizontalSlider->value();
        int g = ui->horizontalSlider_2->value();
        int b = ui->horizontalSlider_3->value();
        QPalette color;
        color.setColor(QPalette::WindowText, QColor(r, g, b));
        ui->label_11->setPalette(color);
        ui->label_12->setPalette(color);
        ui->label_13->setPalette(color);

        m_default_scene.slots_text_color_changed(QColor(r,g,b));
    });

    connect(ui->spinBox_font, &QSpinBox::valueChanged, [=](int _a){
        int r = ui->horizontalSlider->value();
        int g = ui->horizontalSlider_2->value();
        int b = ui->horizontalSlider_3->value();
        QFont tmp = ui->fontComboBox->currentFont();
        QPalette color;
        color.setColor(QPalette::WindowText, QColor(r, g, b));
        tmp.setPixelSize(_a);
        m_default_scene.slots_text_font_changed(tmp);
        ui->label_11->setFont(tmp);
        ui->label_11->setPalette(color);
        ui->label_12->setFont(tmp);
        ui->label_12->setPalette(color);
        ui->label_13->setFont(tmp);
        ui->label_13->setPalette(color);
    });

    // init other inspector TODO.


    // init the scene
    m_default_scene.set_tool_type(painters::tool_type::None);
    ui->tabWidget_2->setCurrentIndex(0);
}

void MainWindow::slots_tree_node_update() {
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(1);
    painters::p_graphic_layer_stack &tmp_stack = m_default_scene.m_layer_stack;
    for (auto &layer: tmp_stack) {
        QTreeWidgetItem *top_item = new QTreeWidgetItem(ui->treeWidget);

        top_item->setText(0, layer->get_name().c_str());
        ui->treeWidget->addTopLevelItem(top_item);

        std::map<std::string, QGraphicsItem*> &tmp_nodes = layer->get_nodes();

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
    QString dir_path = QFileDialog::getSaveFileName(this, "choose file name and directory", "C:/Users/Administrator/Desktop", "*.png *.xpm *.jpg");
    QBrush m_brush;

    if (dir_path.isEmpty()) return;

    m_default_scene.m_rectItem->setVisible(false);
    if (m_default_scene.m_reference_line) m_default_scene.m_reference_line->setVisible(false);
    if (m_default_scene.m_reference_line_v) {
        m_brush.setColor(QColor(0, 0, 0, 64));
        m_brush.setStyle(Qt::BrushStyle::NoBrush);
        m_default_scene.m_rectItem->setBrush(m_brush);
        this->update();
        m_default_scene.m_reference_line_v->setVisible(false);
    }

    QImage image(gv->scene()->width(), gv->scene()->height(), QImage::Format_ARGB32);
    QString pngName = dir_path;
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    gv->scene()->render(&painter);
    bool saveSuccess =  image.save(pngName);

    m_default_scene.m_rectItem->setVisible(true);
    if (m_default_scene.m_reference_line) m_default_scene.m_reference_line->setVisible(true);
    if (m_default_scene.m_reference_line_v) {
        m_default_scene.m_reference_line_v->setVisible(true);
        m_brush.setStyle(Qt::BrushStyle::CrossPattern);
        m_default_scene.m_rectItem->setBrush(m_brush);
        this->update();

    }

    Q_ASSERT(saveSuccess == true);
}

void MainWindow::slots_change_table_to_pen(bool enable) {
    ui->tabWidget_2->setCurrentIndex(0);
}

void MainWindow::slots_change_table_to_shape(bool enable) {
    ui->tabWidget_2->setCurrentIndex(2);
}

void MainWindow::slots_change_table_to_image(bool enable) {
    ui->tabWidget_2->setCurrentIndex(3);
}

void MainWindow::slots_change_shape_type(const QString& _a) {
    if (_a == "Rect") {
        slots_change_shape_to_rect();
    }
    else if (_a == "Circle") {
        slots_change_shape_to_circle();
    }
}

void MainWindow::slots_change_shape_to_rect() {
    m_default_scene.set_tool_type(painters::tool_type::Rect);
}

void MainWindow::slots_change_shape_to_circle() {
    m_default_scene.set_tool_type(painters::tool_type::Circle);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gv;
}
