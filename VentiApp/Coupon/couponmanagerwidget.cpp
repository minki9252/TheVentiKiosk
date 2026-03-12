#include "couponmanagerwidget.h"
#include "ui_couponmanagerwidget.h"
#include "couponinputview.h"
#include "couponselectview.h"
#include "couponresultview.h"
#include <QDebug>

CouponManagerWidget::CouponManagerWidget(const QList<KioskData>& cartList, int totalAmount, QWidget *parent)
    : QWidget(parent) // 🌟 QWidget 초기화
    , ui(new Ui::CouponManagerWidget)
{
    ui->setupUi(this);
    
    // 1. 세 개의 화면 부품 생성
    CouponSelectView *selectView = new CouponSelectView(cartList, totalAmount, this);
    CouponInputView *inputView = new CouponInputView(this);
    CouponResultView *resultView = new CouponResultView(this); 
    
    // 2. 스택에 화면 추가
    ui->stackedWidget->addWidget(selectView); // Index 0
    ui->stackedWidget->addWidget(inputView);  // Index 1
    ui->stackedWidget->addWidget(resultView); // Index 2
    
    ui->stackedWidget->setCurrentWidget(selectView);
    
    // 🌟 3. 선택창 신호 연결 (this->accept(), reject() 대신 emit으로 신호 발송)
    connect(selectView, &CouponSelectView::cancelRequested, this, [this](){
        emit paymentCanceled(); // 결제창 닫기 신호 발송
    }); 
    connect(selectView, &CouponSelectView::payRequested, this, [this](){
        emit stepCompleted();   // 다음 결제 단계로 넘어가기 신호 발송
    });    
    connect(selectView, &CouponSelectView::typeSelected, this, &CouponManagerWidget::onTypeSelected);

    // 4. 입력창 신호 연결
    connect(inputView, &CouponInputView::backRequested, this, [this](){
        ui->stackedWidget->setCurrentIndex(0); 
    });

    connect(inputView, &CouponInputView::inputConfirmed, this, [this, resultView](QString code){
        qDebug() << "입력된 쿠폰 번호:" << code;
        resultView->setResult(true);
        ui->stackedWidget->setCurrentIndex(2); 
    });

    // 5. 결과창 신호 연결
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
    ui->stackedWidget->setCurrentIndex(1);
}

void CouponManagerWidget::onSkipRequested() 
{
    // 쿠폰 안 씀! 다음 단계로 넘어가라는 신호 발송
    emit stepCompleted(); 
}
