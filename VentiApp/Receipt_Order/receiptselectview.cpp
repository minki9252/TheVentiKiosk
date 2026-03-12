#include "receiptselectview.h"
#include "ui_receiptselectview.h"

ReceiptSelectView::ReceiptSelectView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReceiptSelectView)
{
    ui->setupUi(this);

    // 가이드라인 2번: emit 호출만, 로직 없음
    connect(ui->btn_yes, &QPushButton::clicked,
            this, &ReceiptSelectView::yesSelected);

    connect(ui->btn_no, &QPushButton::clicked,
            this, &ReceiptSelectView::noSelected);
}

ReceiptSelectView::~ReceiptSelectView()
{
    delete ui;
}
