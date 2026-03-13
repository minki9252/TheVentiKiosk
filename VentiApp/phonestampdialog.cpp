#include "phonestampdialog.h"
#include "ui_phonestampdialog.h"
#include <QMessageBox>
#include <QPushButton>

PhoneStampDialog::PhoneStampDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhoneStampDialog)
{
    ui->setupUi(this);

    // 1. 숫자 버튼들 (0~9) 연결
    QList<QPushButton*> buttons = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };
    for(QPushButton* btn : buttons) {
        connect(btn, &QPushButton::clicked, this, &PhoneStampDialog::onKeyClicked);
    }

    // 2. [추가] 010 버튼 연결
    connect(ui->btn010, &QPushButton::clicked, this, [=](){
        if(phoneNumber.isEmpty()) { // 번호가 비어있을 때만 010 입력
            phoneNumber = "010";
            updateDisplay();
        }
    });

    // 3. [추가] 지우기 버튼 연결
    connect(ui->btnDelete, &QPushButton::clicked, this, [=](){
        if(!phoneNumber.isEmpty()) {
            phoneNumber.chop(1); // 마지막 한 글자 제거
            updateDisplay();
        }
    });

    // 건너뛰기/취소 연결
    connect(ui->btnSkip, &QPushButton::clicked, this, &PhoneStampDialog::reject);
}

PhoneStampDialog::~PhoneStampDialog() { delete ui; }


void PhoneStampDialog::updateDisplay() {
    QString display = phoneNumber;
    // 전화번호 형식에 맞게 하이픈(-) 자동 삽입
    if(display.length() > 3) display.insert(3, "-");
    if(display.length() > 8) display.insert(8, "-");

    ui->lblPhoneDisplay->setText(display); // 화면 라벨에 출력
}

void PhoneStampDialog::onKeyClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn && phoneNumber.length() < 11) {
        phoneNumber += btn->text();
        updateDisplay(); // 공통 함수 호출
    }
}

void PhoneStampDialog::on_btnSave_clicked() {
    if(phoneNumber.length() == 11) {
        QMessageBox::information(this, "적립 완료", "스탬프 적립이 완료되었습니다.");
        accept(); // 성공 닫기
    } else {
        QMessageBox::warning(this, "알림", "번호를 모두 입력해주세요.");
    }
}

void PhoneStampDialog::on_btnSkip_clicked() {
    reject(); // 건너뛰기/취소 시 닫기
}
