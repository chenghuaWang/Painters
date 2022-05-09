#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView_main->setScene(&m_default_scene);
    ui->graphicsView_main->setMaximumSize(QSize(1128, 800));
    ui->graphicsView_main->setMinimumSize(QSize(1128, 800));

}

MainWindow::~MainWindow()
{
    delete ui;
}
