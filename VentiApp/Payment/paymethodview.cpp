#include "paymethodview.h"
#include "ui_paymethodview.h"

PayMethodView::PayMethodView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayMethodView)
{
    ui->setupUi(this);
}

PayMethodView::~PayMethodView()
{
    delete ui;
}
