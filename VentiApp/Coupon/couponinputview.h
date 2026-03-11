#ifndef COUPONINPUTVIEW_H
#define COUPONINPUTVIEW_H

#include <QWidget>

namespace Ui {
class CouponInputView;
}

class CouponInputView : public QWidget
{
    Q_OBJECT

public:
    explicit CouponInputView(QWidget *parent = nullptr);
    ~CouponInputView();

private:
    Ui::CouponInputView *ui;
};

#endif // COUPONINPUTVIEW_H
