#include "paymentmaindialog.h"
#include "ui_paymentmaindialog.h"
#include "couponmanagerwidget.h"
#include "pointmanagerwidget.h"
#include "paymanagerwidget.h"
#include "receiptmanagerwidget.h"
#include <QDebug>

PaymentMainDialog::PaymentMainDialog(const QList<KioskData> &cartList, int totalAmount, QWidget *parent)
    : QDialog(parent), ui(new Ui::PaymentMainDialog)
{
    ui->setupUi(this);
    // 1. 4개의 매니저 객체를 생성합니다. (장바구니 데이터를 CouponManager에 전달)

    // 2. 메인 스택위젯에 순서대로 차곡차곡 집어넣습니다. (주석 해제)

    CouponManagerWidget *step1_Coupon = new CouponManagerWidget(cartList, totalAmount, this);
    PointManagerWidget *step2_Point = new PointManagerWidget(cartList, this);
    PayManagerWidget *step3_Pay = new PayManagerWidget(this);
    ReceiptManagerWidget *step4_Receipt = new ReceiptManagerWidget(this);

    ui->stackedWidget->addWidget(step1_Coupon);
    ui->stackedWidget->addWidget(step2_Point);
    ui->stackedWidget->addWidget(step3_Pay);
    ui->stackedWidget->addWidget(step4_Receipt);
    ui->stackedWidget->setCurrentWidget(step1_Coupon);

    // [쿠폰 완료/건너뜀] → 포인트 적립 단계로
    connect(step1_Coupon, &CouponManagerWidget::readyToNext,
            this, [this, step2_Point]()
            { ui->stackedWidget->setCurrentWidget(step2_Point); });

    // [포인트 적립 완료] → 결제 단계로
    connect(step2_Point, &PointManagerWidget::stepCompleted,
            this, [this, step3_Pay]()
            { ui->stackedWidget->setCurrentWidget(step3_Pay); });

    // [결제 완료] → 영수증 단계로
    connect(step3_Pay, &PayManagerWidget::payStepCompleted,
            this, [this, step4_Receipt]()
            { ui->stackedWidget->setCurrentWidget(step4_Receipt); });

    // [결제 실패/취소] → 창 닫기 (장바구니 유지)
    // reject() → mainwindow의 exec()가 Rejected 반환 → clearCart() 호출 안 됨
    connect(step3_Pay, &PayManagerWidget::paymentAborted,
            this, &QDialog::reject);

    connect(step4_Receipt, &ReceiptManagerWidget::receiptStepCompleted,
            this, &QDialog::accept);

    // 3. 결제창이 켜지면 무조건 첫 번째 단계(쿠폰)부터 보여주도록 설정합니다.
    ui->stackedWidget->setCurrentWidget(step1_Coupon);

    // 🌟 4. CouponManagerWidget에서 보내는 신호를 처리하여 화면 전환 또는 종료 수행
    connect(step1_Coupon, &CouponManagerWidget::stepCompleted, this, &PaymentMainDialog::onCouponStepCompleted);
    connect(step1_Coupon, &CouponManagerWidget::paymentCanceled, this, &PaymentMainDialog::cancelPayment);

    // 🌟 5. 핵심: 쿠폰 매니저에서 'discountApplied(할인금액)' 신호가 오면 내 'applyDiscount' 함수 실행!
    connect(step1_Coupon, &CouponManagerWidget::discountApplied, this, &PaymentMainDialog::applyDiscount);
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

// 🌟 6. 실제로 금액을 차감하고 화면에 갱신하는 함수 구현
void PaymentMainDialog::applyDiscount(int discountAmount)
{
    if (discountAmount <= 0)
        return;

    // 총 금액에서 할인 금액 빼기
    m_totalAmount -= discountAmount;

    // 혹시 할인 금액이 총액보다 커서 마이너스가 되면 0원으로 맞춤
    if (m_totalAmount < 0)
    {
        m_totalAmount = 0;
    }

    qDebug() << "쿠폰 할인 적용됨! 남은 최종 결제 금액:" << m_totalAmount;

    // 🌟 중요: UI의 '총 결제 금액' 텍스트를 업데이트해 주세요!
    // (ui 파일에 만들어둔 총액 표시 라벨 이름이 label_totalPrice라고 가정한 코드입니다. 실제 이름으로 바꿔주세요)
    // ui->label_totalPrice->setText(QString("%1원").arg(m_totalAmount));
}
