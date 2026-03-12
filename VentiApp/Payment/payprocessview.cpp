#include "payprocessview.h"
#include "ui_payprocessview.h"
#include <QPushButton>  // ✅ 추가

PayProcessView::PayProcessView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayProcessView)
{
    ui->setupUi(this);

    // 가이드라인 2번: emit 호출만, 타이머/카운트다운 로직은 Manager가 담당
    connect(ui->btn_insert_card, &QPushButton::clicked,
            this, &PayProcessView::cardInserted);

    connect(ui->btn_cancel, &QPushButton::clicked,
            this, &PayProcessView::cancelled);
}

PayProcessView::~PayProcessView()
{
    delete ui;
}

// ── Public Setters ─────────────────────────────────────────────────────────
// Manager가 데이터를 넘겨주면 View는 표시만 함, 계산 없음

void PayProcessView::setCardName(const QString &cardName)
{
    ui->label_card_name->setText(cardName + " 카드를 삽입해주세요");
}

void PayProcessView::setCountdown(int seconds)
{
    ui->label_countdown->setText(QString::number(seconds) + "초");
    ui->progressBar->setValue(seconds);
}

void PayProcessView::showLoading()
{
    ui->label_status->setText("카드 인식 중...");
    ui->btn_insert_card->setEnabled(false);
    ui->btn_cancel->setEnabled(false);
}

void PayProcessView::showFailed()
{
    ui->label_status->setText("카드 인식에 실패했습니다\n잠시 후 처음 화면으로 돌아갑니다");
    ui->btn_insert_card->setEnabled(false);
    ui->btn_cancel->setEnabled(false);
}

void PayProcessView::showCancelled()
{
    ui->label_status->setText("결제가 취소되었습니다");
    ui->btn_insert_card->setEnabled(false);
    ui->btn_cancel->setEnabled(false);
}
