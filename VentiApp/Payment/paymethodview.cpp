#include "paymethodview.h"
#include "ui_paymethodview.h"
#include <QPushButton>  // ✅ 추가

PayMethodView::PayMethodView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayMethodView)
{
    ui->setupUi(this);
    connectButtons();
}

PayMethodView::~PayMethodView()
{
    delete ui;
}

// 가이드라인 2번: emit 호출만 있어야 함, 상태 변경/조건 분기 금지
void PayMethodView::connectButtons()
{
    // 카드사 버튼 6개 → 어떤 카드가 선택됐는지만 통보
    connect(ui->btn_shinhan,  &QPushButton::clicked, this, [this](){ emit cardSelected("신한카드"); });
    connect(ui->btn_kb,       &QPushButton::clicked, this, [this](){ emit cardSelected("KB국민카드"); });
    connect(ui->btn_hyundai,  &QPushButton::clicked, this, [this](){ emit cardSelected("현대카드"); });
    connect(ui->btn_samsung,  &QPushButton::clicked, this, [this](){ emit cardSelected("삼성카드"); });
    connect(ui->btn_lotte,    &QPushButton::clicked, this, [this](){ emit cardSelected("롯데카드"); });
    connect(ui->btn_hana,     &QPushButton::clicked, this, [this](){ emit cardSelected("하나카드"); });

    // 취소 버튼
    connect(ui->btn_cancel,   &QPushButton::clicked, this, &PayMethodView::cancelled);
}
