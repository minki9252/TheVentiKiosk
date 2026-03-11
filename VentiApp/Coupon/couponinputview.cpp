#include "couponinputview.h"
#include "ui_couponinputview.h"
#include <QPushButton> // 🌟 sender()에서 버튼 객체를 캐스팅하기 위해 필요

CouponInputView::CouponInputView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CouponInputView)
{
    ui->setupUi(this);

    // 1. 초기 실행 시 "번호 입력 칸"이라는 글자를 지워 깔끔하게 만듭니다.
    ui->lineEdit->clear();

    // 2. 🌟 여러 개의 숫자 버튼을 단 하나의 슬롯(onNumberButtonClicked)으로 연결합니다.
    connect(ui->pushButton_10, &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 1
    connect(ui->pushButton,    &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 2
    connect(ui->pushButton_11, &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 3
    connect(ui->pushButton_4,  &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 4
    connect(ui->pushButton_12, &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 5
    connect(ui->pushButton_7,  &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 6
    connect(ui->pushButton_3,  &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 7
    connect(ui->pushButton_9,  &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 8
    connect(ui->pushButton_2,  &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 9
    connect(ui->pushButton_5,  &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 0
    connect(ui->pushButton_13, &QPushButton::clicked, this, &CouponInputView::onNumberButtonClicked); // 010
}

CouponInputView::~CouponInputView()
{
    delete ui;
}

// 🌟 숫자 및 010 버튼 클릭 공통 처리
void CouponInputView::onNumberButtonClicked()
{
    // 방금 클릭된 버튼이 어떤 버튼인지 찾아냅니다 (sender 사용)
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    
    if (button) {
        // 기존 텍스트에 클릭된 버튼의 글자(예: "1", "010")를 이어 붙입니다.
        QString currentText = ui->lineEdit->text();
        ui->lineEdit->setText(currentText + button->text());
    }
}

// 🌟 부분삭제 버튼 (맨 뒤 글자 하나만 지우기)
void CouponInputView::on_pushButton_6_clicked()
{
    QString currentText = ui->lineEdit->text();
    if (!currentText.isEmpty()) {
        currentText.chop(1); // 문자열의 맨 끝 1글자를 잘라냅니다.
        ui->lineEdit->setText(currentText);
    }
}

// 🌟 전체삭제 버튼 (입력칸 초기화)
void CouponInputView::on_pushButton_8_clicked()
{
    ui->lineEdit->clear();
}

// 취소 버튼 누름 -> 뒤로가기
void CouponInputView::on_pushButton_14_clicked()
{
    ui->lineEdit->clear(); // 뒤로 갈 때 입력해둔 번호를 초기화
    emit backRequested();
}

// 입력 버튼 누름 -> 확인
void CouponInputView::on_pushButton_15_clicked()
{
    QString enteredCode = ui->lineEdit->text();
    
    // 입력된 값이 있을 때만 신호를 보냅니다.
    if (!enteredCode.isEmpty()) {
        emit inputConfirmed(enteredCode);
        ui->lineEdit->clear(); // 전송 후 다음 사람을 위해 입력칸 비우기
    }
}