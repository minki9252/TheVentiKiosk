#ifndef COUPONSELECTVIEW_H
#define COUPONSELECTVIEW_H

#include <QWidget>

namespace Ui {
class CouponSelectView;
}

class CouponSelectView : public QWidget
{
    Q_OBJECT

public:
    explicit CouponSelectView(QWidget *parent = nullptr);
    ~CouponSelectView();

signals:
    // 쿠폰 종류(1: 앱/휴대폰, 2: 기프트/모바일)를 선택했다고 알리는 신호
    void typeSelected(int type);

    // 쿠폰 사용 안 함을 선택했다고 알리는 신호
    void skipRequested();

private slots:
    // UI에서 지어주신 버튼 이름과 똑같이 맞춘 함수들입니다!
    void on_coupon_app_btn_clicked();
    void on_coupon_gift_btn_clicked();
    void on_not_use_btn_clicked();

private:
    Ui::CouponSelectView *ui;
};

#endif // COUPONSELECTVIEW_H
