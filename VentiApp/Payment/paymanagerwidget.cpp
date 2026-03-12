#include "paymanagerwidget.h"
#include "ui_paymanagerwidget.h"
#include "paymethodview.h"
#include "payprocessview.h"
#include <QStackedWidget>
#include <QVBoxLayout>

static const int COUNTDOWN_SEC = 30;  // 카운트다운 초
static const int LOADING_MS    = 1500; // 카드 인식 중 표시 시간
static const int ABORT_MS      = 2500; // 실패/취소 후 닫기까지 대기 시간

PayManagerWidget::PayManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayManagerWidget)
    , m_countdown(COUNTDOWN_SEC)
{
    ui->setupUi(this);

    // ── View 생성 ──────────────────────────────────────────────────────────
    m_methodView  = new PayMethodView(this);
    m_processView = new PayProcessView(this);

    // ── StackedWidget 코드로 직접 생성 ────────────────────────────────────
    // Index 0: 카드사 선택 화면
    // Index 1: 카드 삽입 대기 화면
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(m_methodView);   // Index 0
    m_stackedWidget->addWidget(m_processView);  // Index 1
    m_stackedWidget->setCurrentWidget(m_methodView);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    // ── 타이머 생성 ────────────────────────────────────────────────────────
    // 가이드라인 3번: 타이머 로직은 Manager가 소유
    m_countdownTimer = new QTimer(this);
    m_countdownTimer->setInterval(1000); // 1초마다 tick
    connect(m_countdownTimer, &QTimer::timeout, this, &PayManagerWidget::onCountdownTick);

    m_loadingTimer = new QTimer(this);
    m_loadingTimer->setSingleShot(true);
    m_loadingTimer->setInterval(LOADING_MS);
    connect(m_loadingTimer, &QTimer::timeout, this, &PayManagerWidget::onLoadingFinished);

    m_abortTimer = new QTimer(this);
    m_abortTimer->setSingleShot(true);
    m_abortTimer->setInterval(ABORT_MS);
    connect(m_abortTimer, &QTimer::timeout, this, &PayManagerWidget::paymentAborted);

    // ── View 신호 연결 ─────────────────────────────────────────────────────
    connectViews();
}

PayManagerWidget::~PayManagerWidget()
{
    delete ui;
}

// ── Private ────────────────────────────────────────────────────────────────

void PayManagerWidget::connectViews()
{
    // MethodView 신호 → Manager 슬롯
    connect(m_methodView, &PayMethodView::cardSelected,
            this, &PayManagerWidget::onCardSelected);
    connect(m_methodView, &PayMethodView::cancelled,
            this, &PayManagerWidget::onCancelled);

    // ProcessView 신호 → Manager 슬롯
    connect(m_processView, &PayProcessView::cardInserted,
            this, &PayManagerWidget::onCardInserted);
    connect(m_processView, &PayProcessView::cancelled,
            this, &PayManagerWidget::onCancelled);
}

void PayManagerWidget::startCountdown()
{
    m_countdown = COUNTDOWN_SEC;
    m_processView->setCountdown(m_countdown);
    m_countdownTimer->start();
}

void PayManagerWidget::stopAllTimers()
{
    m_countdownTimer->stop();
    m_loadingTimer->stop();
    m_abortTimer->stop();
}

// ── Slots ──────────────────────────────────────────────────────────────────

void PayManagerWidget::onCardSelected(const QString &cardName)
{
    // 1. 선택된 카드사 이름 저장 (상태는 Manager가 소유)
    m_selectedCard = cardName;

    // 2. ProcessView에 카드사 이름 주입 후 화면 전환
    m_processView->setCardName(m_selectedCard);
    m_stackedWidget->setCurrentWidget(m_processView);

    // 3. 카운트다운 시작
    startCountdown();
}

void PayManagerWidget::onCardInserted()
{
    // 카드 삽입 → 카운트다운 멈추고 로딩 표시
    stopAllTimers();
    m_processView->showLoading();

    // 1.5초 후 결제 완료 신호 emit
    m_loadingTimer->start();
}

void PayManagerWidget::onCancelled()
{
    // 취소 → 모든 타이머 멈추고 취소 메시지 표시
    stopAllTimers();
    m_processView->showCancelled();

    // 2.5초 후 paymentAborted 신호 emit → PaymentMainDialog가 창 닫음
    m_abortTimer->start();
}

void PayManagerWidget::onCountdownTick()
{
    m_countdown--;
    m_processView->setCountdown(m_countdown);

    if (m_countdown <= 0) {
        // 30초 초과 → 타이머 멈추고 실패 메시지 표시
        stopAllTimers();
        m_processView->showFailed();

        // 2.5초 후 paymentAborted 신호 emit → PaymentMainDialog가 창 닫음
        m_abortTimer->start();
    }
}

void PayManagerWidget::onLoadingFinished()
{
    // 로딩 완료 → 결제 완료 신호 발행
    // PaymentMainDialog가 수신해서 다음 단계(영수증)로 전환
    emit payStepCompleted();
}
