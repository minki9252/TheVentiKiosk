#ifndef KIOSKEVENT_H
#define KIOSKEVENT_H

#include <QString>
#include <QStringList>
#include <QVariant>

enum KioskAction {
    CATEGORY_COFFEE,
    CATEGORY_BEVERAGE,
    CATEGORY_DESSERT,
    MENU_SELECT_ITEM,
    MENU_CANCEL_ITEM,
    CART_ADD,
    CART_REMOVE,
    CART_INCREASE_QTY,
    CART_DECREASE_QTY,
    CART_CLEAR_ALL,
    CART_CHECKOUT
};

struct KioskEvent {
    KioskAction action;
    int menuId = -1;
    QString menuName;
    QString categoryName;
    int basePrice = 0;
    QString beanType;
    QString size;
    int extraShots = 0;
    QStringList toppings;
    QString whipAmount;
    int quantity = 1;
    int optionPrice = 0;
    int totalPrice = 0;
    QVariant extraData;
};

#endif // KIOSKEVENT_H