#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include <QWidget>

namespace Ui {
class about_window;
}

class about_window : public QWidget
{
    Q_OBJECT

public:
    explicit about_window(QWidget *parent = nullptr);
    ~about_window();

private:
    Ui::about_window *ui;
};

#endif // ABOUT_WINDOW_H
