#include "categorywidget.h"
#include "ui_categorywidget.h"

categorywidget::categorywidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::categorywidget)
{
    ui->setupUi(this);
}

categorywidget::~categorywidget()
{
    delete ui;
}
