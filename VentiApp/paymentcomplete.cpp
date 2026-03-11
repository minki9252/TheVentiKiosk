#include "paymentcomplete.h"
#include "ui_paymentcomplete.h"

paymentcomplete::paymentcomplete(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::paymentcomplete)
{
    ui->setupUi(this);
}

paymentcomplete::~paymentcomplete()
{
    delete ui;
}
