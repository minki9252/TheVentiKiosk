#ifndef CARTWIDGET_H
#define CARTWIDGET_H

#include <QWidget>

namespace Ui {
class cartwidget;
}

class cartwidget : public QWidget
{
    Q_OBJECT

public:
    explicit cartwidget(QWidget *parent = nullptr);
    ~cartwidget();

private:
    Ui::cartwidget *ui;
};

#endif // CARTWIDGET_H
