#ifndef PAYMENTMAINDIALOG_H
#define PAYMENTMAINDIALOG_H

#include <QDialog>
#include <QList>
#include "KioskData.h"

namespace Ui { class PaymentMainDialog; }

class PaymentMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentMainDialog(const QList<KioskData>& cartList,
                               int totalAmount,
                               QWidget *parent = nullptr);
    ~PaymentMainDialog();

private:
    Ui::PaymentMainDialog *ui;
};

#endif // PAYMENTMAINDIALOG_H
