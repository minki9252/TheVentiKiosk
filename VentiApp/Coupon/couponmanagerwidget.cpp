#include "couponmanagerwidget.h"
#include "ui_couponmanagerwidget.h"
#include "couponselectview.h"
#include <QMessageBox>

CouponManagerWidget::CouponManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CouponManagerWidget)
{
    ui->setupUi(this);
    CouponSelectView * selectView = new CouponSelectView(this);
    ui -> stackedWidget -> addWidget(selectView);
    ui -> stackedWidget -> setCurrentWidget(selectView);
    connect(selectView, &CouponSelectView::typeSelected, this, &CouponManagerWidget::onTypeSelected);
    connect(selectView, &CouponSelectView::skipRequested, this, &CouponManagerWidget::onSkipRequested);
}

void CouponManagerWidget::onTypeSelected(int type)
{
    if (type == 1) {
        QMessageBox::information(this, "매니저의 대답", "휴대폰 쿠폰을 선택했군요! 다음은 번호 입력 화면입니다.");
    } else if (type == 2) {
        QMessageBox::information(this, "매니저의 대답", "모바일 쿠폰을 선택했군요! 바코드 스캔 화면으로 갈게요.");
    }
}

void CouponManagerWidget::onSkipRequested()
{
    QMessageBox::information(this, "매니저의 대답", "쿠폰 안 쓰시는군요! 바로 포인트 적립으로 넘어갑니다.");
}


CouponManagerWidget::~CouponManagerWidget()
{
    delete ui;
}
