#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ///< the main draw window settings.
    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->insertTab(0, gv, "Main drawing pad");
    gv->setScene(&m_default_scene);
    // ui->graphicsView_main->setScene(&m_default_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}
