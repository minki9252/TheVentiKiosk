#include "pointinputview.h"
#include "ui_pointinputview.h"

PointInputView::PointInputView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PointInputView)
{
    ui->setupUi(this);
}

PointInputView::~PointInputView()
{
    delete ui;
}
