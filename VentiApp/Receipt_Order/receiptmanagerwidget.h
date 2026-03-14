#ifndef RECEIPTMANAGERWIDGET_H
#define RECEIPTMANAGERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QStackedWidget>  // ✅ 추가

namespace Ui { class ReceiptManagerWidget; }

class ReceiptSelectView;
class OrderNumberView;

class ReceiptManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptManagerWidget(QWidget *parent = nullptr);
    ~ReceiptManagerWidget();

signals:
    // PaymentMainDialog가 구독 → accept()로 창 닫기 (장바구니 비우기)
    void receiptStepCompleted();

private slots:
    // 가이드라인 3번: View 신호를 받아 실제 제어 로직 수행
    void onYesSelected();          // 영수증 출력 선택
    void onNoSelected();           // 영수증 없이 진행
    void onCloseRequested();       // 닫기 버튼 클릭
    void onAutoCloseTimeout();     // 3초 자동 닫힘

private:
    Ui::ReceiptManagerWidget *ui;

    ReceiptSelectView *m_selectView;    // 영수증 출력 여부 선택 화면
    OrderNumberView   *m_orderView;     // 결제완료 + 주문번호 화면

    QTimer *m_autoCloseTimer;           // 3초 자동 닫힘 타이머
    QStackedWidget *m_stackedWidget;

    static int m_currentOrderNumber;

    void connectViews();
    QString generateOrderNumber();      // 주문번호 생성 (Manager 책임)
};

#endif // RECEIPTMANAGERWIDGET_H
