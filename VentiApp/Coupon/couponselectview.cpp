#include "couponselectview.h"
#include "ui_couponselectview.h"

CouponSelectView::CouponSelectView(const QList<KioskData>& cartList, int totalAmount, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CouponSelectView),
    m_cartList(cartList),
    m_totalAmount(totalAmount) // 🌟 여기서 인자로 받은 값이 멤버 변수로 복사됩니다.
{
    ui->setupUi(this);

    // 디버깅을 위해 생성 시점의 금액을 로그로 찍어보세요.
    // 만약 여기서도 이상한 숫자가 뜬다면 부모(Manager)에서 잘못 보낸 것입니다.
    qDebug() << "CouponSelectView 생성 - 초기 금액:" << m_totalAmount;

    populateList();
}

CouponSelectView::~CouponSelectView() { delete ui; }

void CouponSelectView::populateList() {
    ui->listOrderDetails->clear();
    for (const KioskData &item : m_cartList) {
        QString orderText = QString("%1 | %2\n수량: %3개 | 금액: %4원")
                                .arg(item.menuName).arg(item.summaryText)
                                .arg(item.quantity).arg(item.totalPrice * item.quantity);
        ui->listOrderDetails->addItem(orderText);
    }
    
    // (선택) 만약 생성될 때도 총 금액을 라벨에 띄우고 싶다면 여기에 추가하세요.
    // ui->label_totalPrice->setText(QString("%1원").arg(m_totalAmount));
}

// 🌟 2. 새로 추가한 할인 적용 함수 구현
void CouponSelectView::applyDiscount(int discountAmount)
{
    if (discountAmount <= 0) return;

    // 1. 계산 수행
    m_totalAmount -= discountAmount;
    if (m_totalAmount < 0) m_totalAmount = 0;

    // 2. ✅ 정상적인 로그 출력 (이 줄만 남게 됩니다)
    qDebug() << "쿠폰 할인 적용 완료! 최종 결제 금액:" << m_totalAmount << "원";

    // 3. UI 리스트 업데이트
    ui->listOrderDetails->addItem("------------------------------------");
    QString discountText = QString("[쿠폰 할인] -%1원\n결제 예정 금액: %2원")
                               .arg(discountAmount)
                               .arg(m_totalAmount);

    ui->listOrderDetails->addItem(discountText);
    ui->listOrderDetails->scrollToBottom();
}

// 🌟 버튼 클릭 시 중재자에게 신호(emit)만 던집니다. (창을 직접 닫지 않음)
void CouponSelectView::on_btnCancel_clicked() { emit cancelRequested(); }
void CouponSelectView::on_btnPay_clicked() { emit payRequested(); }

// (쿠폰 버튼들도 마찬가지)
void CouponSelectView::on_coupon_app_btn_clicked() { emit typeSelected(1); }
void CouponSelectView::on_coupon_gift_btn_clicked() { emit typeSelected(2); }
void CouponSelectView::on_not_use_btn_clicked() { emit skipRequested(); }

void CouponSelectView::on_btnUseCoupon_clicked() {
    // 내부 UI가 바뀌거나 로직이 있다면 여기서 처리 (예: 쿠폰 영역 보이기)
}
