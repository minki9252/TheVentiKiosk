#include "orderconfirmdialog.h"
#include "ui_orderconfirmdialog.h"
#include "coupondialog.h"

OrderConfirmDialog::OrderConfirmDialog(const QList<KioskData>& cartList, int totalAmount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderConfirmDialog),
    m_cartList(cartList),
    m_totalAmount(totalAmount)
{
    ui->setupUi(this);

    // 창이 열리자마자 리스트에 장바구니 내역을 채워 넣습니다.
    populateList();
}

OrderConfirmDialog::~OrderConfirmDialog()
{
    delete ui;
}

void OrderConfirmDialog::populateList()
{
    ui->listOrderDetails->clear();

    // 장바구니 데이터를 순회하며 리스트에 텍스트로 추가
    for (const KioskData &item : m_cartList) {
        QString orderText = QString("%1 | %2\n수량: %3개 | 금액: %4원")
                                .arg(item.menuName)
                                .arg(item.summaryText)
                                .arg(item.quantity)
                                .arg(item.totalPrice * item.quantity);

        ui->listOrderDetails->addItem(orderText);
    }

    // 총 결제 금액 라벨이 있다면 업데이트 (ui에 lblFinalPrice를 만드셨다면 주석 해제)
    // ui->lblFinalPrice->setText(QString("최종 결제 금액: %1원").arg(m_totalAmount));
}

// 취소 버튼 클릭 시 (메모: 2p로 돌아감)
void OrderConfirmDialog::on_btnCancel_clicked()
{
    this->reject(); // 창을 닫고 취소 신호를 보냄
}

// 구매/결제 버튼 클릭 시
void OrderConfirmDialog::on_btnPay_clicked()
{
    this->accept(); // 창을 닫고 결제 성공 신호를 보냄
}

// 쿠폰 사용 버튼 클릭 시 (메모: 쿠폰 입력 모달로 이동)
void OrderConfirmDialog::on_btnUseCoupon_clicked()
{
    // 1. 쿠폰 모달창 생성 (부모를 this로 설정하여 이 창 위로 뜨게 함)
    CouponDialog couponDialog(this);
    
    // 2. 쿠폰 모달창 띄우기
    if (couponDialog.exec() == QDialog::Accepted) {
        // 성공적으로 쿠폰을 조회하고 확인을 누르고 닫았을 때
        qDebug() << "쿠폰 사용 완료 (또는 사용 안함 선택)";
        
        // 여기에 쿠폰 할인 금액을 적용하여 m_totalAmount를 수정하고,
        // 최종 결제 금액 라벨(lblFinalPrice)을 갱신하는 코드를 추가할 수 있습니다.
        // 예: m_totalAmount -= 1000;
        //     ui->lblFinalPrice->setText(...);
    } else {
        // 쿠폰 창에서 취소를 누르거나 강제로 닫았을 때
        qDebug() << "쿠폰 사용 취소됨";
    }
}
