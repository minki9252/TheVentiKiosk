#ifndef PAYPROCESSVIEW_H
#define PAYPROCESSVIEW_H

#include <QWidget>

namespace Ui { class PayProcessView; }

class PayProcessView : public QWidget
{
    Q_OBJECT

public:
    explicit PayProcessView(QWidget *parent = nullptr);
    ~PayProcessView();

    // Manager가 선택된 카드사 이름을 주입하는 setter
    void setCardName(const QString &cardName);

    // Manager가 카운트다운 숫자를 갱신할 때 호출
    void setCountdown(int seconds);

    // Manager가 로딩 메시지를 표시할 때 호출
    void showLoading();

    // Manager가 실패 메시지를 표시할 때 호출
    void showFailed();

    // Manager가 취소 메시지를 표시할 때 호출
    void showCancelled();

signals:
    // 가이드라인 2번: View는 "무슨 일이 일어났는지"만 emit
    void cardInserted();  // "카드 삽입" 버튼 클릭 (실제 카드 삽입 재연)
    void cancelled();     // 취소 버튼 클릭

private:
    Ui::PayProcessView *ui;
};

#endif // PAYPROCESSVIEW_H
