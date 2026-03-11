#ifndef COUPONMANAGERWIDGET_H
#define COUPONMANAGERWIDGET_H

#include <QWidget>

namespace Ui {
class CouponManagerWidget;
}

class CouponManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CouponManagerWidget(QWidget *parent = nullptr);
    ~CouponManagerWidget();

private slots:
    void onTypeSelected(int type);
    void onSkipRequested();

private:
    Ui::CouponManagerWidget *ui;
};

#endif // COUPONMANAGERWIDGET_H
