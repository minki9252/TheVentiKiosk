#include "beverage.h"
#include "ui_beverage.h"

beverage::beverage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::beverage)
{
    ui->setupUi(this);
}

beverage::~beverage()
{
    delete ui;
}
