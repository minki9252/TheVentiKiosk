#include "paymentmaindialog.h"
#include "ui_paymentmaindialog.h"
#include "couponmanagerwidget.h"
#include "pointmanagerwidget.h"
#include "paymanagerwidget.h"
#include "receiptmanagerwidget.h"
#include <QDebug>

PaymentMainDialog::PaymentMainDialog(const QList<KioskData>& cartList, int totalAmount, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PaymentMainDialog)
{
    ui->setupUi(this);
    
    // 1. 4개의 매니저 객체를 생성합니다. (장바구니 데이터를 CouponManager에 전달)
    CouponManagerWidget* step1_Coupon = new CouponManagerWidget(cartList, totalAmount, this);
    PointManagerWidget* step2_Point = new PointManagerWidget(this);
    PayManagerWidget* step3_Pay = new PayManagerWidget(this);
    ReceiptManagerWidget* step4_Receipt = new ReceiptManagerWidget(this);

    // 2. 메인 스택위젯에 순서대로 차곡차곡 집어넣습니다. (주석 해제)
    ui->stackedWidget->addWidget(step1_Coupon);   // Index 0
    ui->stackedWidget->addWidget(step2_Point);    // Index 1
    ui->stackedWidget->addWidget(step3_Pay);      // Index 2
    ui->stackedWidget->addWidget(step4_Receipt);  // Index 3

    // 3. 결제창이 켜지면 무조건 첫 번째 단계(쿠폰)부터 보여주도록 설정합니다.
    ui->stackedWidget->setCurrentWidget(step1_Coupon);

    // 🌟 4. CouponManagerWidget에서 보내는 신호를 처리하여 화면 전환 또는 종료 수행
    connect(step1_Coupon, &CouponManagerWidget::stepCompleted, this, &PaymentMainDialog::onCouponStepCompleted);
    connect(step1_Coupon, &CouponManagerWidget::paymentCanceled, this, &PaymentMainDialog::cancelPayment);
}

PaymentMainDialog::~PaymentMainDialog()
{
    delete ui;
}

void PaymentMainDialog::onCouponStepCompleted()
{
    qDebug() << "쿠폰 단계 완료. 포인트 결제 단계로 이동합니다.";
    // 다음 단계(Point) 화면으로 스택 위젯 인덱스 변경
    ui->stackedWidget->setCurrentIndex(1);
}

void PaymentMainDialog::cancelPayment()
{
    qDebug() << "결제 취소 신호 수신. 결제창을 닫습니다.";
    this->reject(); // QDialog 닫기 (취소)
}