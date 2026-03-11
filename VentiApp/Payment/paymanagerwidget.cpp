#include "paymanagerwidget.h"
#include "ui_paymanagerwidget.h"

PayManagerWidget::PayManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayManagerWidget)
{
    ui->setupUi(this);
}

PayManagerWidget::~PayManagerWidget()
{
    delete ui;
}
