#ifndef MENUDATALOADER_H
#define MENUDATALOADER_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include "databasemanager.h" // MenuData 구조체가 정의된 곳

class MenuDataLoader {
public:
    static bool loadFromJson(const QString& filePath, QStringList& categories, QList<MenuData>& menus) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) return false; // 파일 열기 실패 시 false 반환

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        // JSON 배열을 순회하며 데이터 객체화
        for (auto v : root["categories"].toArray()) categories << v.toString();
        for (auto v : root["menus"].toArray()) {
            QJsonObject obj = v.toObject();
            menus.append({obj["category"].toString(), obj["name"].toString(),
                          obj["price"].toInt(), obj["image"].toString()});
        }
        return true;
    }
};

#endif
