#include "couponresultview.h"
#include "ui_couponresultview.h"

CouponResultView::CouponResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CouponResultView),
    m_discountAmount(0) // 초기화
{
    ui->setupUi(this);
}

CouponResultView::~CouponResultView()
{
    delete ui;
}

// 🌟 성공/실패 여부에 따라 할인 금액 설정
void CouponResultView::setResult(const QString &couponCode)
{
    // 중요: 매번 실행 시 할인 금액을 0으로 초기화하고 시작
    m_discountAmount = 0;

    // 1. 이미 사용된 쿠폰인지 확인
    if (m_usedCoupons.contains(couponCode)) {
        ui->stackedWidget->setCurrentIndex(1); // 실패 화면
        return;
    }

    // 2. 번호 일치 확인
    if (couponCode == "1234-5678-9123") {
        m_usedCoupons.insert(couponCode);
        m_discountAmount = 2000;          // 성공 금액 설정
        ui->stackedWidget->setCurrentIndex(0); // 성공 화면
    }
    else {
        ui->stackedWidget->setCurrentIndex(1); // 실패 화면
    }
}

void CouponResultView::on_pushButton_clicked()
{
    emit resultConfirmed(m_discountAmount); // 성공 시 2000 전달
}

void CouponResultView::on_pushButton_2_clicked()
{
    emit resultConfirmed(m_discountAmount); // 실패 시 0 전달
}
