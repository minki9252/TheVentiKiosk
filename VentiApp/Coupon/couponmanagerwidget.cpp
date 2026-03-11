#include "couponmanagerwidget.h"
#include "ui_couponmanagerwidget.h"
#include "couponinputview.h"
#include "couponselectview.h"
#include <QMessageBox>
#include <QDebug>

CouponManagerWidget::CouponManagerWidget(const QList<KioskData>& cartList, int totalAmount, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CouponManagerWidget)
{
    ui->setupUi(this);
    
    // 1. 두 개의 화면 부품 생성
    CouponSelectView *selectView = new CouponSelectView(cartList, totalAmount, this);
    CouponInputView *inputView = new CouponInputView(this);
    
    // 2. 스택에 화면 추가 (Index 0: 주문확인/선택, Index 1: 쿠폰번호입력)
    ui->stackedWidget->addWidget(selectView); // Index 0
    ui->stackedWidget->addWidget(inputView);  // Index 1
    
    // 첫 화면은 선택창으로 고정
    ui->stackedWidget->setCurrentWidget(selectView);
    
    // 3. 🌟 선택창(selectView) 신호 연결
    connect(selectView, &CouponSelectView::cancelRequested, this, &QDialog::reject); // 팝업 닫기(취소)
    connect(selectView, &CouponSelectView::payRequested, this, &QDialog::accept);    // 팝업 닫기(결제 진행)
    connect(selectView, &CouponSelectView::typeSelected, this, &CouponManagerWidget::onTypeSelected);
    // connect(selectView, &CouponSelectView::skipRequested, ... ); (필요 시 연결)

    // 4. 🌟 입력창(inputView) 신호 연결
    // 4-1. 입력창에서 취소(뒤로가기)를 누르면? -> 다시 0번(선택창) 화면으로 전환
    connect(inputView, &CouponInputView::backRequested, this, [this](){
        ui->stackedWidget->setCurrentIndex(0); 
    });

    // 4-2. 입력창에서 확인을 누르면? -> 쿠폰 적용 로직 실행 후 결제 승인
    connect(inputView, &CouponInputView::inputConfirmed, this, [this](QString code){
        qDebug() << "입력된 쿠폰 번호:" << code;
        
        // 사용자에게 적용 완료 메시지 띄우기
        QMessageBox::information(this, "알림", "쿠폰 (" + code + ") 적용 완료!");
        
        // 💡 향후 추가할 로직:
        // 여기서 DB나 서버를 조회해 할인 금액을 계산하고, 
        // selectView(주문 내역 화면)의 총 금액 라벨을 갱신해주는 코드를 넣으면 완벽합니다.

        // 🌟 핵심 수정: 창을 닫지 않고 다시 0번(주문 확인창) 화면으로 되돌립니다!
        ui->stackedWidget->setCurrentIndex(0); 
    });
}

CouponManagerWidget::~CouponManagerWidget() { delete ui; }


void CouponManagerWidget::onTypeSelected(int type)
{
    qDebug() << "선택된 쿠폰 타입:" << type;
    
    // type(1: APP, 2: 기프티콘) 상관없이 번호 입력창(Index 1)으로 화면을 넘깁니다!
    ui->stackedWidget->setCurrentIndex(1);
}

void CouponManagerWidget::onSkipRequested() {
    QMessageBox::information(this, "매니저", "쿠폰 안 씀! 결제 진행을 위해 창을 닫습니다.");
    this->accept(); // 결제 진행을 위해 승인하고 닫음
}