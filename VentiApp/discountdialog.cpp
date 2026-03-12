#include "discountdialog.h"
#include "ui_discountdialog.h" // [중요] incomplete type 에러 해결의 핵심

DiscountDialog::DiscountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiscountDialog)
{
    ui->setupUi(this);
}

DiscountDialog::~DiscountDialog()
{
    delete ui;
}

// 각 쿠폰 선택 시 accept()를 호출하여 MainWindow에 결과 전달
void DiscountDialog::on_btnAppCoupon_clicked() { accept(); }
void DiscountDialog::on_btnMobileCoupon_clicked() { accept(); }
void DiscountDialog::on_btnCancel_clicked() { accept(); }
