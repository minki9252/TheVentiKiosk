#include "ordernumberview.h"
#include "ui_ordernumberview.h"

OrderNumberView::OrderNumberView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OrderNumberView)
{
    ui->setupUi(this);

    // 가이드라인 2번: emit만, 3초 자동닫힘 타이머는 Manager가 담당
    connect(ui->btn_close, &QPushButton::clicked,
            this, &OrderNumberView::closeRequested);
}

OrderNumberView::~OrderNumberView()
{
    delete ui;
}

// Manager가 주문번호를 계산해서 넘겨주면 View는 그대로 표시만 함
void OrderNumberView::setOrderNumber(const QString &orderNum)
{
    ui->label_order_number->setText("주문번호 " + orderNum + "번");
}
