#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

void loadMenuData()
{
    QFile file(":/data/menus.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "JSON 파일을 열 수 없습니다.";
        return;
    }

    // 전체 데이터를 읽어와서 JSON 문서로 변환
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    // 루트 객체 가져오기
    QJsonObject root = doc.object();

    // 카테고리 배열 추출
    QJsonArray categoryArray = root["categories"].toArray();
    for (int i = 0; i < categoryArray.size(); ++i)
    {
        qDebug() << "카테고리:" << categoryArray[i].toString();
    }

    // 메뉴 상세 배열 추출
    QJsonArray menuArray = root["menus"].toArray();
    for (int i = 0; i < menuArray.size(); ++i)
    {
        QJsonObject menuObj = menuArray[i].toObject();
        QString name = menuObj["name"].toString();
        int price = menuObj["price"].toInt();
        qDebug() << "메뉴명:" << name << ", 가격:" << price;
    }
}
