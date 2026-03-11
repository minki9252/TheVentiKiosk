#include "pointmanagerwidget.h"
#include "ui_pointmanagerwidget.h"

PointManagerWidget::PointManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PointManagerWidget)
{
    ui->setupUi(this);
}

PointManagerWidget::~PointManagerWidget()
{
    delete ui;
}
