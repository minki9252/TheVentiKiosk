#include "paymentmaindialog.h"
#include "ui_paymentmaindialog.h"
#include "couponmanagerwidget.h"
#include "pointmanagerwidget.h"
#include "paymanagerwidget.h"
#include "receiptmanagerwidget.h"

PaymentMainDialog::PaymentMainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PaymentMainDialog)
{
    ui->setupUi(this);
    // 1. 4개의 매니저 객체를 생성합니다.
    CouponManagerWidget* step1_Coupon = new CouponManagerWidget(this);
    PointManagerWidget* step2_Point = new PointManagerWidget(this);
    PayManagerWidget* step3_Pay = new PayManagerWidget(this);
    ReceiptManagerWidget* step4_Receipt = new ReceiptManagerWidget(this);

    // 2. 메인 스택위젯(상자)에 순서대로 차곡차곡 집어넣습니다.
    ui->stackedWidget->addWidget(step1_Coupon);
    ui->stackedWidget->addWidget(step2_Point);
    ui->stackedWidget->addWidget(step3_Pay);
    ui->stackedWidget->addWidget(step4_Receipt);

    // 3. 결제창이 켜지면 무조건 첫 번째 단계(쿠폰 매니저)부터 보여주도록 설정합니다.
    ui->stackedWidget->setCurrentWidget(step1_Coupon);
}

PaymentMainDialog::~PaymentMainDialog()
{
    delete ui;
}
