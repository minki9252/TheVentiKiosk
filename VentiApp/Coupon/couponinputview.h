#ifndef COUPONINPUTVIEW_H
#define COUPONINPUTVIEW_H

#include <QWidget>

namespace Ui { class CouponInputView; }

class CouponInputView : public QWidget
{
    Q_OBJECT

public:
    explicit CouponInputView(QWidget *parent = nullptr);
    ~CouponInputView();

signals:
    void backRequested();                   // 취소(뒤로가기) 버튼 신호
    void inputConfirmed(QString code);      // 입력 완료 버튼 신호

private slots:
    void on_pushButton_14_clicked(); // 취소 버튼
    void on_pushButton_15_clicked(); // 입력 버튼

    // 🌟 새로 추가된 슬롯들
    void onNumberButtonClicked();    // 0~9, 010 버튼 공통 처리
    void on_pushButton_6_clicked();  // 부분삭제 버튼
    void on_pushButton_8_clicked();  // 전체삭제 버튼

private:
    Ui::CouponInputView *ui;
};

#endif // COUPONINPUTVIEW_H