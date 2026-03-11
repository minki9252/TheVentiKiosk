#include "payprocessview.h"
#include "ui_payprocessview.h"

PayProcessView::PayProcessView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayProcessView)
{
    ui->setupUi(this);
}

PayProcessView::~PayProcessView()
{
    delete ui;
}
