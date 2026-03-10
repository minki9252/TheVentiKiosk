#include "coupondialog.h"
#include "ui_coupondialog.h"

CouponDialog::CouponDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CouponDialog)
{
    ui->setupUi(this);

    // 시작할 때 첫 페이지(쿠폰 사용 여부 묻기)로 설정
    ui->stackedWidget->setCurrentIndex(0);

    // 🌟 숫자 버튼 0~9번을 모두 찾아 하나의 슬롯(onNumButtonClicked)으로 연결
    connect(ui->btnNum0, &QPushButton::clicked, this, &CouponDialog::onNumButtonClicked);
    connect(ui->btnNum1, &QPushButton::clicked, this, &CouponDialog::onNumButtonClicked);
    connect(ui->btnNum2, &QPushButton::clicked, this, &CouponDialog::onNumButtonClicked);
    // ... (3부터 9까지 추가해 주세요)
}

CouponDialog::~CouponDialog() { delete ui; }

// 1. [쿠폰 사용] 버튼 클릭 -> 키패드 화면(페이지 1)으로 이동
void CouponDialog::on_btnUseCoupon_clicked() {
    ui->lblInput->clear(); // 입력창 초기화
    ui->stackedWidget->setCurrentIndex(1);
}

// 2. [사용 안함] 버튼 클릭 -> 결제 진행(모달 정상 종료)
void CouponDialog::on_btnSkipCoupon_clicked() {
    this->accept(); // accept()를 호출하면 성공적으로 창이 닫힘
}

// 3. 키패드 숫자 클릭 시 텍스트 추가
void CouponDialog::onNumButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender()); // 눌린 버튼 찾기
    if (btn) {
        QString currentText = ui->lblInput->text();
        ui->lblInput->setText(currentText + btn->text()); // 기존 텍스트에 숫자 이어붙이기
    }
}

// 4. 키패드 창에서 [취소] 버튼 클릭 -> 다시 첫 화면으로
void CouponDialog::on_btnCancelCoupon_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// 5. 키패드 창에서 [확인] 버튼 클릭 -> DB조회 후 결과 화면(페이지 2)으로 이동
void CouponDialog::on_btnConfirmCoupon_clicked() {
    QString inputCode = ui->lblInput->text();

    // 임시 로직: 1234면 성공, 아니면 실패로 테스트
    if (inputCode == "1234") {
        ui->lblResult->setText("조회 성공!\n할인이 적용되었습니다.");
    } else {
        ui->lblResult->setText("조회 실패!\n유효하지 않은 쿠폰입니다.");
    }

    ui->stackedWidget->setCurrentIndex(2); // 결과 화면으로 전환
}

// 6. 결과 화면에서 [확인] 버튼 클릭
void CouponDialog::on_btnResultOk_clicked() {
    // 성공이든 실패든 창을 닫고 결제 단계로 넘어가려면 accept()
    // (실패 시 다시 입력하게 하려면 setCurrentIndex(1)로 보낼 수도 있습니다)
    this->accept();
}
