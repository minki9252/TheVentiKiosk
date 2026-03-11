#include "receiptselectview.h"
#include "ui_receiptselectview.h"

ReceiptSelectView::ReceiptSelectView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReceiptSelectView)
{
    ui->setupUi(this);
}

ReceiptSelectView::~ReceiptSelectView()
{
    delete ui;
}
