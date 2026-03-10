#ifndef KIOSKDATA_H
#define KIOSKDATA_H

#include <QString>

// 🌟 지훈님이 이름을 바꾸신 새로운 구조체 (메뉴 정보용)
struct KioskData {
    int menuId;
    QString menuName;
    int basePrice;
    QString imagePath;  // ✅ 이게 있어야 함
    int totalPrice;
    int quantity = 1;
    QString sizeName;
    QString beanName;
    QString shotName;
    QString summaryText;
};

#endif
