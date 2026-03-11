#ifndef COUPONRESULTVIEW_H
#define COUPONRESULTVIEW_H

#include <QWidget>

namespace Ui {
class CouponResultView;
}

class CouponResultView : public QWidget
{
    Q_OBJECT

public:
    explicit CouponResultView(QWidget *parent = nullptr);
    ~CouponResultView();

private:
    Ui::CouponResultView *ui;
};

#endif // COUPONRESULTVIEW_H
