#ifndef COUPONMANAGERWIDGET_H
#define COUPONMANAGERWIDGET_H

#include <QWidget> // 🌟 QDialog에서 QWidget으로 다시 원복!
#include <QList>
#include "KioskData.h"

namespace Ui { class CouponManagerWidget; }

class CouponManagerWidget : public QWidget // 🌟 QWidget 상속
{
    Q_OBJECT

public:
    explicit CouponManagerWidget(const QList<KioskData>& cartList, int totalAmount, QWidget *parent = nullptr);
    ~CouponManagerWidget();

signals:
    // 🌟 부모(PaymentMainDialog)에게 현재 단계가 끝났음을 알리는 신호들
    void stepCompleted();    // 다음 단계로 넘어가라!
    void paymentCanceled();  // 결제창 전체를 닫아라!
    void readyToNext();  // ✅ 추가

private slots:
    void onTypeSelected(int type);
    void onSkipRequested();

private:
    Ui::CouponManagerWidget *ui;
};

#endif // COUPONMANAGERWIDGET_H
