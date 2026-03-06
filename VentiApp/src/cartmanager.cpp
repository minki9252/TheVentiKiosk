#include "header/cartmanager.h"

CartManager::CartManager() {}

void CartManager::addItem(const KioskEvent& newItem) {
    bool found = false;
    for (int i = 0; i < m_cartList.size(); ++i) {
        KioskEvent& item = m_cartList[i];
        if (item.menuId == newItem.menuId && item.beanType == newItem.beanType &&
            item.size == newItem.size && item.extraShots == newItem.extraShots) {
            
            item.quantity += newItem.quantity;
            item.totalPrice = (item.basePrice + item.optionPrice) * item.quantity;
            found = true;
            break;
        }
    }
    if (!found) {
        m_cartList.append(newItem);
    }
}

void CartManager::updateQty(int index, int delta) {
    if (index < 0 || index >= m_cartList.size()) return;

    m_cartList[index].quantity += delta;
    if (m_cartList[index].quantity <= 0) {
        removeItem(index);
        return;
    }
    int unitPrice = m_cartList[index].basePrice + m_cartList[index].optionPrice;
    m_cartList[index].totalPrice = unitPrice * m_cartList[index].quantity;
}

void CartManager::removeItem(int index) {
    if (index >= 0 && index < m_cartList.size()) {
        m_cartList.removeAt(index);
    }
}

void CartManager::clear() {
    m_cartList.clear();
}

const QList<KioskEvent>& CartManager::getCartList() const {
    return m_cartList;
}

int CartManager::getTotalPrice() const {
    int total = 0;
    for (const auto& item : m_cartList) total += item.totalPrice;
    return total;
}

int CartManager::getTotalQuantity() const {
    int total = 0;
    for (const auto& item : m_cartList) total += item.quantity;
    return total;
}