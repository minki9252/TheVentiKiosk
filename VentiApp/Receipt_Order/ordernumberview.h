#ifndef ORDERNUMBERVIEW_H
#define ORDERNUMBERVIEW_H

#include <QWidget>

namespace Ui { class OrderNumberView; }

class OrderNumberView : public QWidget
{
    Q_OBJECT

public:
    explicit OrderNumberView(QWidget *parent = nullptr);
    ~OrderNumberView();

    // Manager가 주문번호를 넘겨주면 View는 표시만 함
    void setOrderNumber(const QString &orderNum);

signals:
    // 가이드라인 2번: 닫기 버튼이 눌렸다는 사실만 emit
    void closeRequested();

private:
    Ui::OrderNumberView *ui;
};

#endif // ORDERNUMBERVIEW_H
