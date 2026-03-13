#include "couponselectview.h"
#include "ui_couponselectview.h"

CouponSelectView::CouponSelectView(const QList<KioskData>& cartList, int totalAmount, QWidget *parent) :
    QWidget(parent), // 🌟 QWidget으로 변경
    ui(new Ui::CouponSelectView),
    m_cartList(cartList),
    m_totalAmount(totalAmount)
{
    ui->setupUi(this);
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

    // 1. 내부 금액 차감
    m_totalAmount -= discountAmount;
    if (m_totalAmount < 0) {
        m_totalAmount = 0;
    }

    // 2. 주문 내역 리스트(listOrderDetails) 맨 밑에 할인 항목을 한 줄 텍스트로 추가합니다!
    QString discountText = QString("====================\n[쿠폰 할인 적용]\n할인 금액: -%1원\n최종 결제 금액: %2원")
                                .arg(discountAmount)
                                .arg(m_totalAmount);
    
    ui->listOrderDetails->addItem(discountText);

    // 3. (선택) 리스트 항목이 많을 경우 새로 추가된 할인 내역이 보이도록 스크롤을 맨 아래로 내립니다.
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