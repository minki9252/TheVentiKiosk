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
    connect(ui->btn_shinhan,  &QPushButton::clicked, this, [this](){ emit cardSelected("신용카드"); });
    connect(ui->btn_kb,       &QPushButton::clicked, this, [this](){ emit cardSelected("삼성페이"); });
    connect(ui->btn_hyundai,  &QPushButton::clicked, this, [this](){ emit cardSelected("애플페이"); });
    connect(ui->btn_samsung,  &QPushButton::clicked, this, [this](){ emit cardSelected("카카오페이"); });
    connect(ui->btn_lotte,    &QPushButton::clicked, this, [this](){ emit cardSelected("모바일상품권"); });
    connect(ui->btn_hana,     &QPushButton::clicked, this, [this](){ emit cardSelected("지역화폐"); });

    // 취소 버튼
    connect(ui->btn_cancel, &QPushButton::clicked, this, [this](){
        // this->hide(); // 필요한 경우 즉시 시각적으로 숨김 처리 가능
        emit cancelled();
    });
}
