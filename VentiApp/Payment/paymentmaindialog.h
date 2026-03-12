#ifndef PAYMENTMAINDIALOG_H
#define PAYMENTMAINDIALOG_H

#include <QDialog>
#include <QList>
#include "KioskData.h"

namespace Ui {
class PaymentMainDialog;
}

class PaymentMainDialog : public QDialog
{
    Q_OBJECT

public:
    // 🌟 MainWindow에서 데이터를 받을 수 있도록 생성자 매개변수 추가
    explicit PaymentMainDialog(const QList<KioskData>& cartList, int totalAmount, QWidget *parent = nullptr);
    ~PaymentMainDialog();

private slots:
    void onCouponStepCompleted(); // 쿠폰 단계 완료 시 포인트 단계로 넘어가기 위한 슬롯
    void cancelPayment();         // 결제 전체 취소 슬롯

private:
    Ui::PaymentMainDialog *ui;
};

#endif // PAYMENTMAINDIALOG_H