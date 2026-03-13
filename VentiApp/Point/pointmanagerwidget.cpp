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

    connect(m_inputView, &PointInputView::cancelled, this, &PointManagerWidget::stepCompleted);
    connect(m_resultView, &PointResultView::confirmed, this, &PointManagerWidget::stepCompleted);
}

// // 🌟 [수정된 부분] 마스킹 처리를 제거하고 원본 번호를 그대로 표시합니다.
// void PointManagerWidget::updateInputDisplay()
// {
//     // 기존: m_inputView->setDisplay(QString("*").repeated(m_phoneNum.length()));
//     m_inputView->setDisplay(m_phoneNum); // 입력된 번호 그대로 화면에 출력
// }



// void PointManagerWidget::onDigitPressed(const QString &digit)
// {
//     m_phoneNum += digit;
//     updateInputDisplay();
// }


void PointManagerWidget::updateInputDisplay()
{
    QString displayStr;
    int len = m_phoneNum.length();

    // 1. 입력된 번호가 없으면 빈 화면 처리
    if (len == 0) {
        m_inputView->setDisplay("");
        return;
    }

    // 2. 앞 3자리 (예: 010)
    displayStr += m_phoneNum.left(3);

    // 3. 중간 4자리 마스킹 처리 (입력 길이가 3을 초과했을 때부터)
    if (len > 3) {
        displayStr += "-"; // 첫 번째 하이픈 추가
        int middleLen = qMin(len - 3, 4); // 중간 자리는 최대 4자리까지만
        displayStr += QString("*").repeated(middleLen);
    }

    // 4. 마지막 4자리 그대로 표시 (입력 길이가 7을 초과했을 때부터)
    if (len > 7) {
        displayStr += "-"; // 두 번째 하이픈 추가
        displayStr += m_phoneNum.mid(7); // 8번째 숫자(인덱스 7)부터 끝까지 그대로 붙임
    }

    // 5. 최종 완성된 문자열을 화면에 전달
    m_inputView->setDisplay(displayStr);
}

void PointManagerWidget::onDigitPressed(const QString &digit)
{
    // 입력된 번호가 이미 11자리라면 더 이상 추가하지 않고 함수 종료
    if (m_phoneNum.length() >= 11) {
        return;
    }

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
