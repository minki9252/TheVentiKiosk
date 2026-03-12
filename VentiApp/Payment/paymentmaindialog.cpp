#include "paymentmaindialog.h"
#include "ui_paymentmaindialog.h"
#include "couponmanagerwidget.h"
#include "pointmanagerwidget.h"
#include "paymanagerwidget.h"
#include "receiptmanagerwidget.h"

PaymentMainDialog::PaymentMainDialog(const QList<KioskData>& cartList,
                                     int totalAmount,
                                     QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PaymentMainDialog)
{
    ui->setupUi(this);

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

}


PaymentMainDialog::~PaymentMainDialog()
{
    delete ui;
}
