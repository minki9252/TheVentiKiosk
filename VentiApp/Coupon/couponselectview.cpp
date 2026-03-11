#include "couponselectview.h"
#include "ui_couponselectview.h"

CouponSelectView::CouponSelectView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CouponSelectView)
{
    ui->setupUi(this);
}

CouponSelectView::~CouponSelectView()
{
    delete ui;
}

// 1. 앱(휴대폰) 쿠폰 버튼을 눌렀을 때
void CouponSelectView::on_coupon_app_btn_clicked()
{
    emit typeSelected(1);
}

// 2. 기프트(모바일) 쿠폰 버튼을 눌렀을 때
void CouponSelectView::on_coupon_gift_btn_clicked()
{
    emit typeSelected(2);
}

// 3. 사용 안 함 버튼을 눌렀을 때
void CouponSelectView::on_not_use_btn_clicked()
{
    emit skipRequested();
}
