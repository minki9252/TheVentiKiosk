#include "pointmanagerwidget.h"
#include "ui_pointmanagerwidget.h"
#include "pointinputview.h"
#include "pointresultview.h"

PointManagerWidget::PointManagerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PointManagerWidget)
    , m_phoneNum("")
{
    ui->setupUi(this);

    m_inputView  = new PointInputView(this);
    m_resultView = new PointResultView(this);

    ui->stackedWidget->addWidget(m_inputView);
    ui->stackedWidget->addWidget(m_resultView);
    ui->stackedWidget->setCurrentWidget(m_inputView);

    connectViews();
}

PointManagerWidget::~PointManagerWidget() { delete ui; }

void PointManagerWidget::connectViews()
{
    connect(m_inputView, &PointInputView::digitPressed,   this, &PointManagerWidget::onDigitPressed);
    connect(m_inputView, &PointInputView::deletePressed,  this, &PointManagerWidget::onDeletePressed);
    connect(m_inputView, &PointInputView::clearPressed,   this, &PointManagerWidget::onClearPressed);
    connect(m_inputView, &PointInputView::confirmPressed, this, &PointManagerWidget::onConfirmPressed);
    connect(m_inputView, &PointInputView::cancelled,      this, &PointManagerWidget::cancelled);
    connect(m_resultView, &PointResultView::confirmed,    this, &PointManagerWidget::pointSaveCompleted);
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
    if (!m_phoneNum.isEmpty()) m_phoneNum.chop(1);
    updateInputDisplay();
}

void PointManagerWidget::onClearPressed()
{
    m_phoneNum.clear();
    updateInputDisplay();
}

void PointManagerWidget::onConfirmPressed()
{
    if (m_phoneNum.isEmpty()) return;
    int earnedPoints = 0; // 향후 DB 조회로 교체
    m_resultView->setResult(m_phoneNum, earnedPoints);
    ui->stackedWidget->setCurrentWidget(m_resultView);
}
