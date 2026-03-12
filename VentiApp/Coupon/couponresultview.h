#ifndef COUPONRESULTVIEW_H
#define COUPONRESULTVIEW_H

#include <QWidget>

namespace Ui { class CouponResultView; }

class CouponResultView : public QWidget
{
    Q_OBJECT

public:
    explicit CouponResultView(QWidget *parent = nullptr);
    ~CouponResultView();

    // 🌟 성공/실패 여부를 설정하는 함수
    void setResult(bool isSuccess);

signals:
    void resultConfirmed(); // 확인 버튼 클릭 시 중재자에게 보낼 신호

private slots:
    void on_pushButton_clicked();   // 성공 화면의 '확인' 버튼
    void on_pushButton_2_clicked(); // 실패 화면의 '확인' 버튼

private:
    Ui::CouponResultView *ui;
};

#endif // COUPONRESULTVIEW_H