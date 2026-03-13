#include "receiptmanagerwidget.h"
#include "ui_receiptmanagerwidget.h"
#include "receiptselectview.h"
#include "ordernumberview.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QRandomGenerator>

static const int AUTO_CLOSE_MS = 3000; // 3초 자동 닫힘

ReceiptManagerWidget::ReceiptManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReceiptManagerWidget)
    , m_currentOrderNumber(100) // 100번부터 시작하도록 초기화
{
    ui->setupUi(this);

    // ── View 생성 ──────────────────────────────────────────────────────────
    m_selectView = new ReceiptSelectView(this);
    m_orderView  = new OrderNumberView(this);

    // ── StackedWidget 코드로 직접 생성 ────────────────────────────────────
    // Index 0: 영수증 출력 여부 선택 화면
    // Index 1: 결제완료 + 주문번호 화면
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(m_selectView);  // Index 0
    m_stackedWidget->addWidget(m_orderView);   // Index 1
    m_stackedWidget->setCurrentWidget(m_selectView);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    // ── 자동 닫힘 타이머 생성 ─────────────────────────────────────────────
    // 가이드라인 3번: 타이머 로직은 Manager가 소유
    m_autoCloseTimer = new QTimer(this);
    m_autoCloseTimer->setSingleShot(true);
    m_autoCloseTimer->setInterval(AUTO_CLOSE_MS);
    connect(m_autoCloseTimer, &QTimer::timeout,
            this, &ReceiptManagerWidget::onAutoCloseTimeout);

    // ── View 신호 연결 ─────────────────────────────────────────────────────
    connectViews();
}

ReceiptManagerWidget::~ReceiptManagerWidget()
{
    delete ui;
}

// ── Private ────────────────────────────────────────────────────────────────

void ReceiptManagerWidget::connectViews()
{
    // SelectView 신호 → Manager 슬롯
    connect(m_selectView, &ReceiptSelectView::yesSelected,
            this, &ReceiptManagerWidget::onYesSelected);
    connect(m_selectView, &ReceiptSelectView::noSelected,
            this, &ReceiptManagerWidget::onNoSelected);

    // OrderView 신호 → Manager 슬롯
    connect(m_orderView, &OrderNumberView::closeRequested,
            this, &ReceiptManagerWidget::onCloseRequested);
}

QString ReceiptManagerWidget::generateOrderNumber()
{
    // 가이드라인 3번: 데이터 계산은 Manager 책임
    // 기존 랜덤 로직을 제거하고 호출될 때마다 1씩 증가시킨 후 문자열로 반환
    return QString::number(m_currentOrderNumber++);
}

// ── Slots ──────────────────────────────────────────────────────────────────

void ReceiptManagerWidget::onYesSelected()
{
    // 영수증 출력 선택 → 주문번호 생성 후 결제완료 화면으로 전환
    // 향후: 실제 영수증 출력 로직 추가 자리
    QString orderNum = generateOrderNumber();
    m_orderView->setOrderNumber(orderNum);
    m_stackedWidget->setCurrentWidget(m_orderView);

    // 3초 자동 닫힘 타이머 시작
    m_autoCloseTimer->start();
}

void ReceiptManagerWidget::onNoSelected()
{
    // 영수증 없이 진행 → 주문번호만 표시 후 결제완료 화면으로 전환
    QString orderNum = generateOrderNumber();
    m_orderView->setOrderNumber(orderNum);
    m_stackedWidget->setCurrentWidget(m_orderView);

    // 3초 자동 닫힘 타이머 시작
    m_autoCloseTimer->start();
}

void ReceiptManagerWidget::onCloseRequested()
{
    // 닫기 버튼 → 타이머 취소 후 즉시 완료 신호 발행
    m_autoCloseTimer->stop();
    emit receiptStepCompleted();
}

void ReceiptManagerWidget::onAutoCloseTimeout()
{
    // 3초 경과 → 자동으로 완료 신호 발행
    emit receiptStepCompleted();
}
