#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QFileInfo>
#include <QSqlError>

// 데이터를 읽어오는 객체
class MenuDataLoader
{
public:
    static bool loadFromJson(const QString &filePath, QStringList &categories, QList<MenuData> &menus)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
            qDebug() << "❌ JSON 파일을 열 수 없습니다. 시도한 경로:" << filePath;
        return false;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        QJsonArray catArray = root["categories"].toArray();
        for (const QJsonValue &v : catArray)
            categories << v.toString();

        QJsonArray menuArray = root["menus"].toArray();
        for (const QJsonValue &v : menuArray)
        {
            QJsonObject obj = v.toObject();
            menus.append(MenuData(
                obj["category"].toString(),
                obj["name"].toString(),
                obj["price"].toInt(),
                obj["image"].toString()));
        }
        return true;
    }
};

void initDatabase()
{
    // SQLite DB 연결 설정
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("venti.db");
}

// static으로 선언된 instance 함수 구현
DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

bool DatabaseManager::initDatabase(const QString &dbName)
{
    // 이미 연결된 DB가 있는지 확인하여 중복 에러 방지
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        db = QSqlDatabase::database(QSqlDatabase::defaultConnection);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    db.setDatabaseName(dbName);

    // DB 열기
    if (!db.open())
    {
        qDebug() << "❌ DB 열기 실패:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ DB 연결 성공!";

    // FK키 활성화
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;");

    // 테이블 생성 및 데이터 삽입
    if (setupDatabase())
    {
        return true;
    }

    return false;
}

DatabaseManager::DatabaseManager()
{
    // 이미 DB 연결이 되어있는지 확인하여 중복 생성 방지
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        db = QSqlDatabase::database(QSqlDatabase::defaultConnection);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    QString dbPath = QCoreApplication::applicationDirPath() + "/venti.db";
    db.setDatabaseName(dbPath);

    qDebug() << "현재 사용 중인 DB 절대 경로:" << dbPath;
}

bool DatabaseManager::setupDatabase()
{
    // 앱이 실행되는 동안 단 한 번만 실행되도록 막아주는 방어막
    static bool isSetupDone = false;
    if (isSetupDone) {
        return true; // 이미 한 번 세팅했다면 아무것도 안 하고 바로 돌아감
    }
    isSetupDone = true;

    if (!db.open())
    {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;"); // FK 활성화

    // 테이블 생성 쿼리
    query.exec("CREATE TABLE CATEGORIES (category_id INTEGER PRIMARY KEY, category_name VARCHAR(50));");
    query.exec("CREATE TABLE MEMBERS (phone_num VARCHAR(20) PRIMARY KEY, stamp_count INTEGER, total_pay_amount INTEGER);");
    query.exec("CREATE TABLE MENU_INFO (menu_id INTEGER PRIMARY KEY, category_id INTEGER, kr_name VARCHAR(100), price INTEGER, size_info VARCHAR(50), product_type VARCHAR(20), is_soldout INTEGER, image_path VARCHAR(255), etc_info TEXT, FOREIGN KEY (category_id) REFERENCES CATEGORIES(category_id));");
    query.exec("CREATE TABLE ORDERS (order_id INTEGER PRIMARY KEY, member_phone VARCHAR(20), order_date DATETIME, total_amount INTEGER, pay_method VARCHAR(30), is_takeout INTEGER, FOREIGN KEY (member_phone) REFERENCES MEMBERS(phone_num));");
    query.exec("CREATE TABLE ORDER_ITEMS (item_id INTEGER PRIMARY KEY, order_id INTEGER, menu_id INTEGER, quantity INTEGER, subtotal INTEGER, selected_options TEXT, FOREIGN KEY (order_id) REFERENCES ORDERS(order_id), FOREIGN KEY (menu_id) REFERENCES MENU_INFO(menu_id));");
    query.exec("CREATE TABLE OPTION_GROUPS ("
               "group_id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "group_name VARCHAR(50), " // 예: "사이즈", "원두"
               "is_exclusive INTEGER DEFAULT 1);"); // 1이면 단일 선택(Radio), 0이면 중복 가능(Check)

    // 세부 옵션과 가격
    query.exec("CREATE TABLE OPTIONS ("
               "option_id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "group_id INTEGER, "
               "option_name VARCHAR(50), "
               "extra_price INTEGER DEFAULT 0, "
               "FOREIGN KEY (group_id) REFERENCES OPTION_GROUPS(group_id));");
    qDebug() << "DB 테이블 세팅 완료";

    // 테이블 생성 호출
    insertInitialData();

    return true;
}

bool DatabaseManager::insertInitialData()
{
    QSqlQuery query;
    // 기존 데이터 싹 비우기
    query.exec("DELETE FROM MENU_INFO");
    query.exec("DELETE FROM CATEGORIES");
    query.exec("DELETE FROM OPTIONS");
    query.exec("DELETE FROM OPTION_GROUPS");
    query.exec("DELETE FROM sqlite_sequence WHERE name='CATEGORIES' OR name='MENU_INFO' OR name='OPTION_GROUPS' OR name='OPTIONS'");

    // 사이즈
    query.exec("INSERT INTO OPTION_GROUPS (group_name, is_exclusive) VALUES ('사이즈', 1)");
    int sizeId = query.lastInsertId().toInt();
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, 'Large(600ml)', 0)").arg(sizeId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, 'Jumbo(960ml)', 1000)").arg(sizeId));

    // 원두
    query.exec("INSERT INTO OPTION_GROUPS (group_name, is_exclusive) VALUES ('원두', 1)");
    int beanId = query.lastInsertId().toInt();
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '시그니처', 0)").arg(beanId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '다크원두', 0)").arg(beanId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '디카페인', 800)").arg(beanId));

    // 샷 선택
    query.exec("INSERT INTO OPTION_GROUPS (group_name, is_exclusive) VALUES ('샷 선택', 1)");
    int shotId = query.lastInsertId().toInt();
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '기본(2샷)', 0)").arg(shotId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '연하게(1샷)', 0)").arg(shotId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '시그니처샷추가', 500)").arg(shotId));

    // 당도/시럽
    query.exec("INSERT INTO OPTION_GROUPS (group_name, is_exclusive) VALUES ('시럽 선택', 1)");
    int syrupId = query.lastInsertId().toInt();
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '달게', 0)").arg(syrupId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '바닐라시럽', 500)").arg(syrupId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '헤이즐넛시럽', 500)").arg(syrupId));

    // 음료 변경
    query.exec("INSERT INTO OPTION_GROUPS (group_name, is_exclusive) VALUES ('음료 변경', 0)");
    int changeId = query.lastInsertId().toInt();
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '탄산수변경', 500)").arg(changeId));

    // 토핑 추가
    query.exec("INSERT INTO OPTION_GROUPS (group_name, is_exclusive) VALUES ('토핑 선택', 0)");
    int toppingId = query.lastInsertId().toInt();
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '아이스크림', 500)").arg(toppingId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '타피오카펄', 900)").arg(toppingId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '화이트펄', 900)").arg(toppingId));
    query.exec(QString("INSERT INTO OPTIONS (group_id, option_name, extra_price) VALUES (%1, '오트팝핑펄', 900)").arg(toppingId));

    QString exePath = QCoreApplication::applicationDirPath();
    QString jsonPath = exePath + "/../../../VentiApp/menus.json";

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "❌ JSON 파일을 열 수 없습니다.";
        return false;
    }

    // 파일 데이터 읽기
    QByteArray fileData = file.readAll();
    qDebug() << "💡 읽어들인 파일 크기:" << fileData.size() << "bytes";

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull())
    {
        qDebug() << "❌ 치명적 에러: JSON 내용 파싱 실패! 쉼표나 괄호가 빠졌는지 확인하세요.";
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray catArray = root["categories"].toArray();
    QJsonArray menuArray = root["menus"].toArray();

    qDebug() << "✅ 발견된 카테고리 개수:" << catArray.size();
    qDebug() << "✅ 발견된 메뉴 개수:" << menuArray.size();

    // 카테고리 DB 삽입
    for (int i = 0; i < catArray.size(); ++i)
    {
        QString catName = catArray[i].toString().trimmed();
        query.prepare("INSERT INTO CATEGORIES (category_name) VALUES (:name)");
        query.bindValue(":name", catName);
        if (query.exec())
        {
            qDebug() << "✔️ DB 카테고리 등록:" << catName;
        }
        else
        {
            qDebug() << "❌ 카테고리 등록 실패:" << query.lastError().text();
        }
    }

    // 3. 메뉴 DB 삽입
    for (int i = 0; i < menuArray.size(); ++i)
    {
        QJsonObject obj = menuArray[i].toObject();
        QString catName = obj["category"].toString().trimmed();
        QString menuName = obj["name"].toString().trimmed();
        int price = obj["price"].toInt();
        QString imgPath = obj["image"].toString().trimmed();

        query.prepare("INSERT INTO MENU_INFO (category_id, kr_name, price, image_path, is_soldout) "
                      "SELECT category_id, :name, :price, :img, 0 FROM CATEGORIES WHERE category_name = :cat");

        query.bindValue(":cat", catName);
        query.bindValue(":name", menuName);
        query.bindValue(":price", price);
        query.bindValue(":img", imgPath);

        if (!query.exec())
        {
            qDebug() << "❌ DB 메뉴 등록 실패 (" << menuName << "):" << query.lastError().text();
        }
    }

    qDebug() << "📢 DB 데이터 삽입 최종 완료!";
    return true;
}

bool DatabaseManager::updateMenuImagePath(const QString &menuName, const QString &newPath)
{
    QSqlQuery query;

    query.prepare("UPDATE MENU_INFO SET image_path = :path WHERE kr_name = :name");
    query.bindValue(":path", newPath);
    query.bindValue(":name", menuName);

    if (!query.exec())
    {
        qDebug() << "경로 업데이트 실패:" << query.lastError().text();
        return false;
    }
    return true;
}

// 카테고리 목록 호출 함수
QStringList DatabaseManager::getCategoryNames()
{
    QStringList list;
    QSqlQuery query("SELECT category_name FROM CATEGORIES ORDER BY category_id");

    while (query.next())
    {
        // DB에서 꺼낸 카테고리 이름을 리스트에 담기
        list.append(query.value(0).toString());
    }
    return list;
}

// 특정 카테고리의 메뉴 호출
QList<QVariantMap> DatabaseManager::getMenusByCategory(const QString &categoryName)
{
    QList<QVariantMap> menuList;
    QSqlQuery query;

    // 조인을 사용해 카테고리 이름으로 해당 메뉴들을 한 번에 찾기
    query.prepare("SELECT kr_name, price, image_path FROM MENU_INFO "
                  "WHERE category_id = (SELECT category_id FROM CATEGORIES WHERE category_name = :catName)");
    query.bindValue(":catName", categoryName);

    if (query.exec())
    {
        while (query.next())
        {
            QVariantMap menu;
            menu["name"] = query.value(0).toString();
            menu["price"] = query.value(1).toInt();
            menu["image"] = query.value(2).toString();
            menuList.append(menu);
        }
    }
    return menuList;
}

QList<OptionGroup> DatabaseManager::getAllOptions()
{
    QList<OptionGroup> groups;
    QSqlQuery groupQuery("SELECT group_id, group_name, is_exclusive FROM OPTION_GROUPS ORDER BY group_id");

    while (groupQuery.next()) {
        OptionGroup gp;
        int gId = groupQuery.value(0).toInt();
        gp.groupName = groupQuery.value(1).toString();
        gp.isExclusive = groupQuery.value(2).toInt() == 1; // 1이면 배타적 선택(Radio)

        QSqlQuery optQuery;
        optQuery.prepare("SELECT option_name, extra_price FROM OPTIONS WHERE group_id = :gid");
        optQuery.bindValue(":gid", gId);
        if (optQuery.exec()) {
            while (optQuery.next()) {
                OptionInfo info;
                info.name = optQuery.value(0).toString();
                info.extraPrice = optQuery.value(1).toInt();
                gp.options.append(info);
            }
        }
        groups.append(gp);
    }
    return groups;
}

// 사용예시
// // 1. 카테고리 버튼 생성 시
// QStringList categories = DatabaseManager::instance().getCategoryNames();
// // categories = ["신메뉴", "커피", "버블티&티", ...] 순서대로 버튼을 만들면 됨

// // 2. "커피" 버튼을 눌렀을 때 메뉴판 갱신
// QList<QVariantMap> coffeeMenus = DatabaseManager::instance().getMenusByCategory("커피");

// for(const auto &item : coffeeMenus) {
//     QString name = item["name"].toString();    // "아메리카노"
//     int price = item["price"].toInt();         // 2000
//     QString img = item["image"].toString();    // ":/Coffe/americano.png"
// }
