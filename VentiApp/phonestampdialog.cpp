#include "phonestampdialog.h"
#include "ui_phonestampdialog.h"
#include <QMessageBox>
#include <QPushButton>

PhoneStampDialog::PhoneStampDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PhoneStampDialog)
{
    ui->setupUi(this);

    // 숫자 버튼들을 리스트로 관리하여 한 번에 연결
    QList<QPushButton*> buttons = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };

    for(QPushButton* btn : buttons) {
        // 숫자 버튼 클릭 시 번호가 입력되도록 연결
        connect(btn, &QPushButton::clicked, this, &PhoneStampDialog::onKeyClicked);
    }

    // 건너뛰기 버튼 클릭 시 창이 닫히도록 연결
    connect(ui->btnSkip, &QPushButton::clicked, this, &PhoneStampDialog::reject);
}

PhoneStampDialog::~PhoneStampDialog() { delete ui; }

void PhoneStampDialog::onKeyClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn && phoneNumber.length() < 11) {
        phoneNumber += btn->text();
        // 010-1234-5678 형식 표시
        QString display = phoneNumber;
        if(display.length() > 3) display.insert(3, "-");
        if(display.length() > 8) display.insert(8, "-");
        ui->lblPhoneDisplay->setText(display);
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
