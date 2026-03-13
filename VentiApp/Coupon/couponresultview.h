#ifndef COUPONRESULTVIEW_H
#define COUPONRESULTVIEW_H

#include <QWidget>
#include <QSet>

namespace Ui { class CouponResultView; }

class CouponResultView : public QWidget
{
    Q_OBJECT

public:
    explicit CouponResultView(QWidget *parent = nullptr);
    ~CouponResultView();

    // 🌟 1. bool 대신 QString을 받도록 수정
    void setResult(const QString &couponCode);

signals:
    // 🌟 2. 확인 버튼을 누를 때 할인 금액(int)을 함께 보내도록 수정
    void resultConfirmed(int discountAmount); 

private slots:
    void on_pushButton_clicked();   
    void on_pushButton_2_clicked(); 

private:
    Ui::CouponResultView *ui;
    // 🌟 3. 할인 금액을 기억할 변수 추가
    int m_discountAmount = 0;
    // 🌟 이미 사용된 쿠폰 번호들을 기억할 집합(Set)
    QSet<QString> m_usedCoupons; 
};

#endif // COUPONRESULTVIEW_H