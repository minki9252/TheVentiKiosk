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