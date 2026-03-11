#ifndef PAYMENTCOMPLETE_H
#define PAYMENTCOMPLETE_H

#include <QDialog>

namespace Ui {
class paymentcomplete;
}

class paymentcomplete : public QDialog
{
    Q_OBJECT

public:
    explicit paymentcomplete(QWidget *parent = nullptr);
    ~paymentcomplete();

private:
    Ui::paymentcomplete *ui;
};

#endif // PAYMENTCOMPLETE_H
