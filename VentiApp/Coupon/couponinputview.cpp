#include "couponinputview.h"
#include "ui_couponinputview.h"

CouponInputView::CouponInputView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CouponInputView)
{
    ui->setupUi(this);
}

CouponInputView::~CouponInputView()
{
    delete ui;
}
