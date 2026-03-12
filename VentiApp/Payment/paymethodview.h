#ifndef PAYMETHODVIEW_H
#define PAYMETHODVIEW_H

#include <QWidget>

namespace Ui { class PayMethodView; }

class PayMethodView : public QWidget
{
    Q_OBJECT

public:
    explicit PayMethodView(QWidget *parent = nullptr);
    ~PayMethodView();

signals:
    // 가이드라인 2번: View는 "무슨 일이 일어났는지"만 emit
    void cardSelected(const QString &cardName); // 카드사 버튼 클릭
    void cancelled();                           // 취소 버튼 클릭

private:
    Ui::PayMethodView *ui;
    void connectButtons();
};

#endif // PAYMETHODVIEW_H
