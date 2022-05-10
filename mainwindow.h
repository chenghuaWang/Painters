#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRect>
#include "P_GraphicViewport.h"
#include "P_CanvasWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    painters::p_graphic_view *gv = new painters::p_graphic_view();
    Ui::MainWindow  *ui;
    painters::p_canvas  m_default_scene;
};
#endif // MAINWINDOW_H
