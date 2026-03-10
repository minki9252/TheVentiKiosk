#include "cartwidget.h"
#include "ui_cartwidget.h"

cartwidget::cartwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cartwidget)
{
    ui->setupUi(this);
}

cartwidget::~cartwidget()
{
    delete ui;
}
