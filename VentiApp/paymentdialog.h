#ifndef PAYMENTDIALOG_H
#define PAYMENTDIALOG_H

#include <QDialog>

namespace Ui { class PaymentDialog; }

class PaymentDialog : public QDialog {
    Q_OBJECT
public:
    explicit PaymentDialog(int total, int discount, QWidget *parent = nullptr);
    ~PaymentDialog();

private slots:
    void onPaymentMethodClicked(); // 결제 수단 버튼 공통 슬롯
    void on_btnFinalPayment_clicked();

private:
    Ui::PaymentDialog *ui;
    int finalAmount; // 최종 결제할 금액
};

#endif
