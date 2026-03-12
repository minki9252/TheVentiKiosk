#include "paymentmaindialog.h"
#include "ui_paymentmaindialog.h"
#include "couponmanagerwidget.h"
#include "pointmanagerwidget.h"
#include "paymanagerwidget.h"
#include "receiptmanagerwidget.h"
#include <QDebug>

<<<<<<< HEAD
PaymentMainDialog::PaymentMainDialog(const QList<KioskData>& cartList,
                                     int totalAmount,
                                     QWidget *parent)
=======
PaymentMainDialog::PaymentMainDialog(const QList<KioskData>& cartList, int totalAmount, QWidget *parent)
>>>>>>> a23c359d2b42293412751b0bf392a26aad20c8bc
    : QDialog(parent)
    , ui(new Ui::PaymentMainDialog)
{
    ui->setupUi(this);
    // 1. 4개의 매니저 객체를 생성합니다. (장바구니 데이터를 CouponManager에 전달)
   
    // 2. 메인 스택위젯에 순서대로 차곡차곡 집어넣습니다. (주석 해제)
   
    CouponManagerWidget*  step1_Coupon  = new CouponManagerWidget(cartList, totalAmount, this);
    PointManagerWidget*   step2_Point   = new PointManagerWidget(this);
    PayManagerWidget*     step3_Pay     = new PayManagerWidget(this);
    ReceiptManagerWidget* step4_Receipt = new ReceiptManagerWidget(this);

    ui->stackedWidget->addWidget(step1_Coupon);
    ui->stackedWidget->addWidget(step2_Point);
    ui->stackedWidget->addWidget(step3_Pay);
    ui->stackedWidget->addWidget(step4_Receipt);
    ui->stackedWidget->setCurrentWidget(step1_Coupon);

    // [쿠폰 완료/건너뜀] → 포인트 적립 단계로
    connect(step1_Coupon, &CouponManagerWidget::readyToNext,
            this, [this, step2_Point](){
                ui->stackedWidget->setCurrentWidget(step2_Point);
            });

    // [포인트 적립 완료] → 결제 단계로
    connect(step2_Point, &PointManagerWidget::pointSaveCompleted,
            this, [this, step3_Pay](){
                ui->stackedWidget->setCurrentWidget(step3_Pay);
            });

    // [포인트 적립 취소] → 결제 단계로
    connect(step2_Point, &PointManagerWidget::cancelled,
            this, [this, step3_Pay](){
                ui->stackedWidget->setCurrentWidget(step3_Pay);
            });

    // [결제 완료] → 영수증 단계로
    connect(step3_Pay, &PayManagerWidget::payStepCompleted,
            this, [this, step4_Receipt](){
                ui->stackedWidget->setCurrentWidget(step4_Receipt);
            });

    // [결제 실패/취소] → 창 닫기 (장바구니 유지)
    // reject() → mainwindow의 exec()가 Rejected 반환 → clearCart() 호출 안 됨
    connect(step3_Pay, &PayManagerWidget::paymentAborted,
            this, &QDialog::reject);

    connect(step4_Receipt, &ReceiptManagerWidget::receiptStepCompleted,
            this, &QDialog::accept);connect(step4_Receipt, &ReceiptManagerWidget::receiptStepCompleted,
            this, &QDialog::accept);

    

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