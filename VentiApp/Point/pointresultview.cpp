#include "pointresultview.h"
#include "ui_pointresultview.h"

PointResultView::PointResultView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PointResultView)
{
    ui->setupUi(this);
}

PointResultView::~PointResultView()
{
    delete ui;
}
