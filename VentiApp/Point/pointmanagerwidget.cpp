#include "pointmanagerwidget.h"
#include "ui_pointmanagerwidget.h"
#include "pointinputview.h"
#include "pointresultview.h"

PointManagerWidget::PointManagerWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PointManagerWidget), m_phoneNum("")
{
    ui->setupUi(this);

    m_inputView = new PointInputView(this);
    m_resultView = new PointResultView(this);

    ui->stackedWidget->addWidget(m_inputView);
    ui->stackedWidget->addWidget(m_resultView);
    ui->stackedWidget->setCurrentWidget(m_inputView);

    connectViews();
}

PointManagerWidget::~PointManagerWidget() { delete ui; }

void PointManagerWidget::connectViews()
{
    connect(m_inputView, &PointInputView::digitPressed, this, &PointManagerWidget::onDigitPressed);
    connect(m_inputView, &PointInputView::deletePressed, this, &PointManagerWidget::onDeletePressed);
    connect(m_inputView, &PointInputView::clearPressed, this, &PointManagerWidget::onClearPressed);
    connect(m_inputView, &PointInputView::confirmPressed, this, &PointManagerWidget::onConfirmPressed);
    // 🌟 [핵심 변경] 취소를 하든, 확인을 하든 외부로는 무조건 "나 끝났어!(stepCompleted)" 라고 통일해서 소리칩니다!
    // (주의: pointmanagerwidget.h 파일의 signals: 에 void stepCompleted(); 를 먼저 선언해 주셔야 합니다)
    connect(m_inputView, &PointInputView::cancelled, this, &PointManagerWidget::stepCompleted);
    connect(m_resultView, &PointResultView::confirmed, this, &PointManagerWidget::stepCompleted);
}

void PointManagerWidget::updateInputDisplay()
{
    m_inputView->setDisplay(QString("*").repeated(m_phoneNum.length()));
}

void PointManagerWidget::onDigitPressed(const QString &digit)
{
    m_phoneNum += digit;
    updateInputDisplay();
}

void PointManagerWidget::onDeletePressed()
{
    if (!m_phoneNum.isEmpty())
        m_phoneNum.chop(1);
    updateInputDisplay();
}

void PointManagerWidget::onClearPressed()
{
    m_phoneNum.clear();
    updateInputDisplay();
}

void PointManagerWidget::onConfirmPressed()
{
    if (m_phoneNum.isEmpty())
        return;
    int earnedPoints = 0; // 향후 DB 조회로 교체
    m_resultView->setResult(m_phoneNum, earnedPoints);
    ui->stackedWidget->setCurrentWidget(m_resultView);
}
