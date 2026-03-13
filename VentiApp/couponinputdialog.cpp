#include "couponinputdialog.h"
#include "ui_couponinputdialog.h"
#include <QMessageBox>
#include <QPushButton>

// 생성자: UI 객체 할당 및 초기 설정
CouponInputDialog::CouponInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CouponInputDialog)
{
    ui->setupUi(this);

    // 숫자 버튼들 연결 (btn0 ~ btn9)
    QList<QPushButton*> buttons = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };
    for(QPushButton* btn : buttons) {
        connect(btn, &QPushButton::clicked, this, &CouponInputDialog::onKeyClicked);
    }

    //지우기 버튼 연결 
    connect(ui->btnDelete, &QPushButton::clicked, this, &CouponInputDialog::on_btnDelete_clicked);

    // 취소 버튼 연결
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

CouponInputDialog::~CouponInputDialog()
{
    delete ui; // 할당된 UI 메모리 해제
}

void CouponInputDialog::on_btnDelete_clicked() {
    if (!currentInput.isEmpty()) {
        currentInput.chop(1); // 문자열의 마지막 한 글자를 제거합니다.
        updateDisplay();      // 변경된 번호를 화면에 다시 그립니다.
    }
}

// 숫자 키패드 클릭 시
void CouponInputDialog::onKeyClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn && currentInput.length() < 12) {
        currentInput += btn->text();
        updateDisplay(); // 여기서 호출!
    }
}

// 확인 버튼 클릭 시 번호 검증
void CouponInputDialog::on_btnConfirm_clicked() {
    // 예시 번호: 123456789012
    if (currentInput == "123456789012") {
        QMessageBox::information(this, "쿠폰 적용", "2,000원 할인이 적용되었습니다.");
        accept(); // 이 신호가 MainWindow로 전달되어 discountAmount가 2000이 됩니다.
    } else {
        QMessageBox::warning(this, "인증 실패", "유효하지 않은 쿠폰 번호입니다.");
        currentInput.clear();
        updateDisplay();
    }
}

void CouponInputDialog::updateDisplay() {
    QString display = currentInput;

    // 4글자마다 하이픈(-) 추가 (예: 1234-5678-9012)
    if(display.length() > 4) display.insert(4, "-");
    if(display.length() > 9) display.insert(9, "-");

    ui->label->setText(display);
}
