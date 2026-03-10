#ifndef CARTWIDGET_H
#define CARTWIDGET_H

#include <QWidget>
#include "KioskData.h" // 추가
#include <QList>       // 추가

namespace Ui
{
    class cartwidget;
}

class cartwidget : public QWidget
{
    Q_OBJECT

public:
    explicit cartwidget(QWidget *parent = nullptr);
    ~cartwidget();
    void updateCart(QList<KioskData> list); // 추가

private:
    Ui::cartwidget *ui;
};

#endif // CARTWIDGET_H
