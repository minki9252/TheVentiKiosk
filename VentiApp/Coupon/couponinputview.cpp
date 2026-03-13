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
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    
    if (button) {
        // 1. 현재 텍스트에서 하이픈(-)을 모두 제거하여 '순수 숫자'만 추출
        QString rawCode = ui->lineEdit->text().remove("-");

        // 2. 쿠폰은 최대 12자리까지만 입력받도록 제한
        if (rawCode.length() >= 12) {
            return; 
        }

        // 3. 클릭된 숫자 추가
        rawCode += button->text();

        // 4. 다시 0000-0000-0000 형태에 맞게 하이픈 조립
        QString formattedCode;
        int len = rawCode.length();

        formattedCode += rawCode.left(4); // 첫 4자리
        
        if (len > 4) {
            formattedCode += "-";
            formattedCode += rawCode.mid(4, 4); // 중간 4자리
        }
        if (len > 8) {
            formattedCode += "-";
            formattedCode += rawCode.mid(8, 4); // 마지막 4자리
        }

        // 5. 화면에 적용
        ui->lineEdit->setText(formattedCode);
    }
}

// 🌟 부분삭제 버튼 (맨 뒤 글자 하나만 지우기)
void CouponInputView::on_pushButton_6_clicked()
{
    // 1. 순수 숫자만 추출
    QString rawCode = ui->lineEdit->text().remove("-");

    if (!rawCode.isEmpty()) {
        // 2. 맨 끝 숫자 1개 지우기
        rawCode.chop(1);

        // 3. 다시 형태에 맞게 조립
        QString formattedCode;
        int len = rawCode.length();

        if (len > 0) {
            formattedCode += rawCode.left(4);
            if (len > 4) {
                formattedCode += "-";
                formattedCode += rawCode.mid(4, 4);
            }
            if (len > 8) {
                formattedCode += "-";
                formattedCode += rawCode.mid(8, 4);
            }
        }

        // 4. 화면에 적용
        ui->lineEdit->setText(formattedCode);
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