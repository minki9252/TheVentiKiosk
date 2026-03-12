#ifndef RECEIPTSELECTVIEW_H
#define RECEIPTSELECTVIEW_H

#include <QWidget>

namespace Ui { class ReceiptSelectView; }

class ReceiptSelectView : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptSelectView(QWidget *parent = nullptr);
    ~ReceiptSelectView();

signals:
    // 가이드라인 2번: View는 "무슨 일이 일어났는지"만 emit
    void yesSelected();  // 네 버튼 → 영수증 출력 요청
    void noSelected();   // 아니오 버튼 → 영수증 없이 다음으로

private:
    Ui::ReceiptSelectView *ui;
};

#endif // RECEIPTSELECTVIEW_H
