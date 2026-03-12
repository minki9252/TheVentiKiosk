#include "couponresultview.h"
#include "ui_couponresultview.h"

CouponResultView::CouponResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CouponResultView)
{
    ui->setupUi(this);
}

CouponResultView::~CouponResultView()
{
    delete ui;
}

// 🌟 DB 조회 등 결과에 따라 내부 스택 화면(성공/실패)을 바꿔주는 함수
void CouponResultView::setResult(bool isSuccess)
{
    if (isSuccess) {
        ui->stackedWidget->setCurrentIndex(0); // page (조회 성공 화면)
    } else {
        ui->stackedWidget->setCurrentIndex(1); // page_2 (조회 실패 화면)
    }
}

// 성공 화면 확인 버튼 눌렀을 때
void CouponResultView::on_pushButton_clicked()
{
    emit resultConfirmed();
}

// 실패 화면 확인 버튼 눌렀을 때
void CouponResultView::on_pushButton_2_clicked()
{
    emit resultConfirmed();
}