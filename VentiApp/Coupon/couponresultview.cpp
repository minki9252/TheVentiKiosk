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
    // 1. 가장 먼저, 이미 사용된 쿠폰인지 확인합니다.
    if (m_usedCoupons.contains(couponCode)) {
        m_discountAmount = 0;    // 할인 불가
        ui->stackedWidget->setCurrentIndex(1); // 실패 화면으로 이동
        // (선택) 실패 화면의 라벨을 "이미 사용된 쿠폰입니다." 로 바꿔줄 수도 있습니다.
        return; // 아래 코드는 실행하지 않고 함수 종료
    }

    // 2. 사용되지 않은 쿠폰이라면, 번호가 일치하는지 확인합니다.
    if (couponCode == "1234-5678-9123") {
        m_usedCoupons.insert(couponCode); // 🌟 핵심: 성공했으므로 '사용됨' 목록에 추가!
        m_discountAmount = 2000;          // 성공: 2000원
        ui->stackedWidget->setCurrentIndex(0); 
    } 
    // 3. 번호가 틀린 경우
    else {
        m_discountAmount = 0;    // 실패: 0원
        ui->stackedWidget->setCurrentIndex(1); 
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