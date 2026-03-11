#ifndef ORDERCONFIRMDIALOG_H
#define ORDERCONFIRMDIALOG_H

#include <QDialog>
#include <QList>
#include "KioskData.h"

namespace Ui {
class OrderConfirmDialog;
}

class OrderConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    // 생성자에서 장바구니 리스트와 총액을 함께 넘겨받습니다.
    explicit OrderConfirmDialog(const QList<KioskData>& cartList, int totalAmount, QWidget *parent = nullptr);
    ~OrderConfirmDialog();

private slots:
    void on_btnCancel_clicked();
    void on_btnPay_clicked();
    void on_btnUseCoupon_clicked();

private:
    Ui::OrderConfirmDialog *ui;
    QList<KioskData> m_cartList; // 전달받은 장바구니 데이터
    int m_totalAmount;           // 전달받은 총액

    void populateList();         // 리스트에 데이터를 채워넣는 함수
};

#endif // ORDERCONFIRMDIALOG_H
