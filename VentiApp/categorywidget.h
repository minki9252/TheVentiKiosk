#ifndef CATEGORYWIDGET_H
#define CATEGORYWIDGET_H

#include <QWidget>

namespace Ui {
class categorywidget;
}

class categorywidget : public QWidget
{
    Q_OBJECT

public:
    explicit categorywidget(QWidget *parent = nullptr);
    ~categorywidget();

private:
    Ui::categorywidget *ui;
};

#endif // CATEGORYWIDGET_H
