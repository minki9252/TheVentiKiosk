#ifndef MENUDATA_H
#define MENUDATA_H

#include <QString>

// 데이터를 담는 순수 객체 (Entity)
struct MenuData {
    QString category;
    QString name;
    int price;
    QString imgPath;
};

#endif // MENUDATA_H
