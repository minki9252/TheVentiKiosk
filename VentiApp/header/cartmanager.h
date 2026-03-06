#ifndef CARTMANAGER_H
#define CARTMANAGER_H

#include <QList>
#include "kioskevent.h" // 공통 이벤트 헤더 포함

class CartManager {
public:
    CartManager();

    // 장바구니 제어 함수들
    void addItem(const KioskEvent& item);
    void removeItem(int index);
    void updateQty(int index, int delta);
    void clear();

    // 장바구니 데이터 가져오기 (UI 갱신용)
    const QList<KioskEvent>& getCartList() const;
    int getTotalPrice() const;
    int getTotalQuantity() const;

private:
    QList<KioskEvent> m_cartList;
};

#endif // CARTMANAGER_H