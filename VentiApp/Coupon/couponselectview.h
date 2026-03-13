#ifndef COUPONSELECTVIEW_H
#define COUPONSELECTVIEW_H

#include <QWidget> // 🌟 QDialog에서 다시 QWidget으로 변경!
#include <QList>
#include "KioskData.h"

namespace Ui { class CouponSelectView; }

class CouponSelectView : public QWidget
{
    Q_OBJECT

public:
    explicit CouponSelectView(const QList<KioskData>& cartList, int totalAmount, QWidget *parent = nullptr);
    ~CouponSelectView();

    void applyDiscount(int discountAmount);
signals: // 🌟 중재자에게 보낼 신호들
    void cancelRequested(); // 취소 눌림
    void payRequested();    // 결제 눌림
    void typeSelected(int type); // 쿠폰 타입 눌림
    void skipRequested();   // 쿠폰 안 씀 눌림

private slots:
    void on_btnCancel_clicked();
    void on_btnPay_clicked();
    void on_btnUseCoupon_clicked();
    void on_coupon_app_btn_clicked();
    void on_coupon_gift_btn_clicked();
    void on_not_use_btn_clicked();

private:
    Ui::CouponSelectView *ui;
    QList<KioskData> m_cartList;
    int m_totalAmount;
    void populateList();
};

#endif // COUPONSELECTVIEW_H