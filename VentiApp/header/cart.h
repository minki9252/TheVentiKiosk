#ifndef CART_H
#define CART_H

#include <QList>
#include "kioskevent.h" // KioskEvent 구조체가 정의된 헤더

class Cart {
public:
    Cart();

    void addItem(const KioskEvent& newItem);
    void updateQty(int index, int delta);
    void removeItem(int index);
    void clear();

    // UI에서 데이터를 가져갈 수 있도록 제공하는 함수
    const QList<KioskEvent>& getList() const;
    int getTotalPrice() const;
    int getTotalQuantity() const;

private:
    QList<KioskEvent> m_cartList;
};

#endif // CART_H