#include "receiptmanagerwidget.h"
#include "ui_receiptmanagerwidget.h"

ReceiptManagerWidget::ReceiptManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReceiptManagerWidget)
{
    ui->setupUi(this);
}

ReceiptManagerWidget::~ReceiptManagerWidget()
{
    delete ui;
}
