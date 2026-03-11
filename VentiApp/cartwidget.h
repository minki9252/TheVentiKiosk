#ifndef CARTWIDGET_H
#define CARTWIDGET_H

#include <QWidget>
#include <QList>
#include "KioskData.h"

namespace Ui { class cartwidget; }

class cartwidget : public QWidget
{
    Q_OBJECT

public:
    explicit cartwidget(QWidget *parent = nullptr);
    ~cartwidget();
    void updateCart(QList<KioskData> list); // 외부에서 아이템 추가
    void clearCart();                       // 외부(결제완료 후)에서 장바구니 비우기

signals:
    // 🌟 추가: 장바구니 총액이 변경될 때마다 부모 창으로 던질 시그널
    void cartTotalChanged(int totalAmount);
    // 🌟 추가: 결제하기 버튼을 눌렀을 때 부모 창으로 던질 시그널
    void checkoutRequested();

private slots:
    void on_btnClearAll_clicked(); // 전체 삭제 버튼 (이름이 btnClearAll 인 경우)
    void on_btnCheckout_clicked(); // 결제 하기 버튼 (이름이 btnCheckout 인 경우)

private:
    Ui::cartwidget *ui;
    QList<KioskData> m_cartList; // 장바구니 진짜 데이터 보관함
    
    void refreshCartUI();
    void removeItem(int index);
    void increaseQty(int index);
    void decreaseQty(int index);
    
    // 🌟 추가: 총액을 계산하고 시그널을 발생시키는 내부 함수
    void calculateTotalAndEmit(); 
};

#endif // CARTWIDGET_H
