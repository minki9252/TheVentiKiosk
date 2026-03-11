#include "ordernumberview.h"
#include "ui_ordernumberview.h"

OrderNumberView::OrderNumberView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OrderNumberView)
{
    ui->setupUi(this);
}

OrderNumberView::~OrderNumberView()
{
    delete ui;
}
