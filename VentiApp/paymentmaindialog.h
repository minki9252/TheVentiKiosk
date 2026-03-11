#ifndef PAYMENTMAINDIALOG_H
#define PAYMENTMAINDIALOG_H

#include <QDialog>

namespace Ui {
class PaymentMainDialog;
}

class PaymentMainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaymentMainDialog(QWidget *parent = nullptr);
    ~PaymentMainDialog();

private:
    Ui::PaymentMainDialog *ui;
};

#endif // PAYMENTMAINDIALOG_H
