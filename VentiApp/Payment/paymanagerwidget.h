#ifndef PAYMANAGERWIDGET_H
#define PAYMANAGERWIDGET_H

// ✅ 추가
#include <QStackedWidget>
#include <QWidget>
#include <QTimer>

namespace Ui { class PayManagerWidget; }

class PayMethodView;
class PayProcessView;

class PayManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PayManagerWidget(QWidget *parent = nullptr);
    ~PayManagerWidget();

signals:
    // PaymentMainDialog가 구독 → 다음 단계(영수증)로 전환
    void payStepCompleted();

    // PaymentMainDialog가 구독 → 창 닫기 (홈으로, 장바구니 유지)
    void paymentAborted();

private slots:
    // 가이드라인 3번: View 신호를 받아 실제 제어 로직 수행
    void onCardSelected(const QString &cardName);  // 카드사 선택됨
    void onCardInserted();                          // 카드 삽입됨
    void onCancelled();                             // 취소 버튼
    void onCountdownTick();                         // 1초 타이머 tick
    void onLoadingFinished();                       // 로딩 1.5초 후 완료

private:
    Ui::PayManagerWidget *ui;

    PayMethodView  *m_methodView;   // 카드사 선택 화면
    PayProcessView *m_processView;  // 카드 삽입 대기 화면

    // ── 상태 변수 (Manager가 단독 소유) ───────────────────────────────────
    QString m_selectedCard;         // 선택된 카드사 이름
    int     m_countdown;            // 남은 카운트다운 초

    QTimer *m_countdownTimer;       // 30초 카운트다운 타이머
    QTimer *m_loadingTimer;         // 카드 인식 중 1.5초 타이머
    QTimer *m_abortTimer;           // 실패/취소 후 2초 타이머

    QStackedWidget *m_stackedWidget;

    void connectViews();
    void startCountdown();
    void stopAllTimers();
};

#endif // PAYMANAGERWIDGET_H
