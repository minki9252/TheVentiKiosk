#include "paymentdialog.h"
#include "ui_paymentdialog.h"
#include "loadingdialog.h"
#include <QMessageBox>

PaymentDialog::PaymentDialog(int total, int discount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaymentDialog)
{
    ui->setupUi(this);

    QLocale ko(QLocale::Korean);
    int toPay = total - discount;
    if(toPay < 0) toPay = 0; // 마이너스 결제 방지

    // 금액 표시 로직
    ui->lblOrderAmount->setText("주문 금액: " + ko.toString(total) + "원");

    // 할인 금액이 있을 때만 강조 표시
    if(discount > 0) {
        ui->lblDiscountAmount->setText("할인 금액: -" + ko.toString(discount) + "원");
        ui->lblDiscountAmount->setStyleSheet("color: #e44d26; font-weight: bold;"); // 강조색
    } else {
        ui->lblDiscountAmount->setText("할인 금액: 0원");
    }

    ui->lblToPayAmount->setText("최종 결제 금액: " + ko.toString(toPay) + "원");

    ui->lblToPayAmount->setStyleSheet(
        "font-size: 14pt; "
        "font-weight: bold; "
        "color: #5d2d91; " // 더벤티 보라색
        "border-top: 2px solid #ddd; "
        "padding-top: 10px;"
        );

    ui->lblOrderAmount->setStyleSheet("font-size: 10pt; color: #666;");
    ui->lblDiscountAmount->setStyleSheet("font-size: 10pt; color: #e44d26;"); // 할인액은 빨간색 계열

    connect(ui->btnCard, &QPushButton::clicked, this, &PaymentDialog::onPaymentMethodClicked);
}

PaymentDialog::~PaymentDialog() { delete ui; }

void PaymentDialog::onPaymentMethodClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    // 선택된 버튼 강조
    btn->setStyleSheet("background-color: #5d2d91; color: white; font-weight: bold; border-radius: 10px;");

    // 로딩 다이얼로그 생성 및 실행
    LoadingDialog loading(this);
    loading.exec();

    // 결제 금액 정보를 업데이트
    QString payAmount = ui->lblToPayAmount->text().replace("최종 결제 금액: ", "");
    ui->lblTotalPaid->setText("결제한 금액: " + payAmount);
    ui->lblToPayAmount->setText("최종 결제 금액: 0원");

    QMessageBox::information(this, "결제 성공", "결제가 정상적으로 완료되었습니다.");

    accept();
}

void PaymentDialog::on_btnFinalPayment_clicked() {
    QMessageBox::information(this, "결제 중", "카드를 삽입하거나 단말기에 태그해주세요.");
    accept(); // 결제 성공 시 창 닫기
}
