#include "couponresultview.h"
#include "ui_couponresultview.h"

CouponResultView::CouponResultView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CouponResultView)
{
    ui->setupUi(this);
}

CouponResultView::~CouponResultView()
{
    delete ui;
}
