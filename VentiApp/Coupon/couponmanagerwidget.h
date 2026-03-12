#ifndef COUPONMANAGERWIDGET_H
#define COUPONMANAGERWIDGET_H

#include <QDialog> // 🌟 QWidget에서 QDialog로 변경!
#include <QList>
#include "KioskData.h"

namespace Ui { class CouponManagerWidget; }

class CouponManagerWidget : public QDialog
{
    Q_OBJECT

public:
    // 🌟 MainWindow로부터 장바구니 데이터를 받아옵니다.
    explicit CouponManagerWidget(const QList<KioskData>& cartList, int totalAmount, QWidget *parent = nullptr);
    ~CouponManagerWidget();

    // 기존 private slots: 위에 아래 블록 추가
signals:
    void readyToNext(); // 쿠폰 단계 완료 → PaymentMainDialog에 통보

private slots:
    void onTypeSelected(int type);
    void onSkipRequested();

private:
    Ui::CouponManagerWidget *ui;
};

#endif // COUPONMANAGERWIDGET_H
