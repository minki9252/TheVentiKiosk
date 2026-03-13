#include "couponinputdialog.h"
#include "ui_couponinputdialog.h" // [중요] 이 줄이 없으면 ui->... 모든 코드에서 에러가 납니다.
#include <QMessageBox>
#include <QPushButton>

// 생성자: UI 객체 할당 및 초기 설정
CouponInputDialog::CouponInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CouponInputDialog) // 여기서 Ui:: 뒤의 이름은 .ui 파일의 objectName과 같아야 합니다.
{
    ui->setupUi(this); // 실제 위젯들을 생성하고 배치합니다.

    // 숫자 버튼들을 리스트로 묶어 이벤트를 연결합니다.
    QList<QPushButton*> buttons = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };

    for(QPushButton* btn : buttons) {
        connect(btn, &QPushButton::clicked, this, &CouponInputDialog::onKeyClicked);
    }

    connect(ui->btnDelete, &QPushButton::clicked, this, &CouponInputDialog::reject);
}

CouponInputDialog::~CouponInputDialog()
{
    delete ui; // 할당된 UI 메모리 해제
}

// 키패드 클릭 시 번호를 currentInput에 저장하고 화면(lblCouponDisplay)에 표시합니다.
void CouponInputDialog::onKeyClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn && currentInput.length() < 12) {
        currentInput += btn->text();

        QString display = currentInput;
        if(display.length() > 4) display.insert(4, "-");
        if(display.length() > 9) display.insert(9, "-");

        ui->lblCouponDisplay->setText(display); // 화면의 라벨 업데이트
    }
}

// 확인 버튼 클릭 시 번호 검증
void CouponInputDialog::on_btnConfirm_clicked()
{
    if (currentInput == "123456789012") {
        QMessageBox::information(this, "성공", "쿠폰이 확인되었습니다.");
        accept(); // 성공 신호를 보내며 창 닫기
    } else {
        QMessageBox::warning(this, "실패", "잘못된 번호입니다.");
        currentInput.clear();
        ui->lblCouponDisplay->clear();
    }
}
