#include "couponmanagerwidget.h"
#include "ui_couponmanagerwidget.h"
#include "couponinputview.h"
#include "couponselectview.h"
#include "couponresultview.h" // 🌟 추가하신 헤더
#include <QDebug>
// <QMessageBox>는 이제 화면 전환으로 대체되므로 제외했습니다.

CouponManagerWidget::CouponManagerWidget(const QList<KioskData>& cartList, int totalAmount, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CouponManagerWidget)
{
    ui->setupUi(this);
    
    // 1. 세 개의 화면 부품 생성 (🌟 결과창 부품 추가됨)
    CouponSelectView *selectView = new CouponSelectView(cartList, totalAmount, this);
    CouponInputView *inputView = new CouponInputView(this);
    CouponResultView *resultView = new CouponResultView(this); 
    
    // 2. 스택에 화면 추가 (Index 0: 주문확인, Index 1: 번호입력, Index 2: 결과확인)
    ui->stackedWidget->addWidget(selectView); // Index 0
    ui->stackedWidget->addWidget(inputView);  // Index 1
    ui->stackedWidget->addWidget(resultView); // Index 2 (🌟 새로 추가된 결과창)
    
    // 첫 화면은 선택창으로 고정
    ui->stackedWidget->setCurrentWidget(selectView);
    
    // 3. 선택창(selectView) 신호 연결
    connect(selectView, &CouponSelectView::cancelRequested, this, &QDialog::reject); // 팝업 닫기(취소)
    connect(selectView, &CouponSelectView::payRequested, this, &QDialog::accept);    // 팝업 닫기(결제 진행)
    connect(selectView, &CouponSelectView::typeSelected, this, &CouponManagerWidget::onTypeSelected);

    // 4. 입력창(inputView) 신호 연결
    // 4-1. 입력창에서 취소(뒤로가기)를 누르면 -> 다시 0번(선택창) 화면으로 전환
    connect(inputView, &CouponInputView::backRequested, this, [this](){
        ui->stackedWidget->setCurrentIndex(0); 
    });

    // 4-2. 🌟 번호를 다 입력하고 '확인'을 누르면 -> 메시지박스 대신 '조회 성공/실패' 창(Index 2) 띄우기
    connect(inputView, &CouponInputView::inputConfirmed, this, [this, resultView](QString code){
        qDebug() << "입력된 쿠폰 번호:" << code;
        
        // 일단 무조건 '성공(true)' 화면이 뜨도록 설정합니다. (나중에 DB 연동 시 이 값을 바꾸면 됨)
        resultView->setResult(true);

        // 결과창(Index 2)으로 화면 전환!
        ui->stackedWidget->setCurrentIndex(2); 
    });

    // 5. 🌟 결과창(resultView) 신호 연결
    // "조회 성공" 화면에서 [확인] 버튼을 누르면 -> 다시 0번(주문내역 선택창)으로 복귀!
    connect(resultView, &CouponResultView::resultConfirmed, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
    });
}

CouponManagerWidget::~CouponManagerWidget() 
{ 
    delete ui; 
}

void CouponManagerWidget::onTypeSelected(int type)
{
    qDebug() << "선택된 쿠폰 타입:" << type;
    
    // type(1: APP, 2: 기프티콘) 상관없이 번호 입력창(Index 1)으로 화면을 넘깁니다!
    ui->stackedWidget->setCurrentIndex(1);
}

void CouponManagerWidget::onSkipRequested() 
{
    // 쿠폰 안 씀! 결제 진행을 위해 창을 닫고 승인
    this->accept(); 
}