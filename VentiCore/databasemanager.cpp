#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>

// 데이터를 읽어오는 객체 (하드코딩 방지용)
class MenuDataLoader {
public:
    static bool loadFromJson(const QString& filePath, QStringList& categories, QList<MenuData>& menus) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) return false;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        QJsonArray catArray = root["categories"].toArray();
        for (const QJsonValue &v : catArray) categories << v.toString();

        QJsonArray menuArray = root["menus"].toArray();
        for (const QJsonValue &v : menuArray) {
            QJsonObject obj = v.toObject();
            menus.append(MenuData(
                obj["category"].toString(),
                obj["name"].toString(),
                obj["price"].toInt(),
                obj["image"].toString()
                ));
        }
        return true;
    }
};

// ✅ 싱글톤 instance 함수
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

// ✅ 생성자: addDatabase는 여기 딱 한 번만
DatabaseManager::DatabaseManager() {
    QString dbPath = QCoreApplication::applicationDirPath() + "/venti.db";

    // ✅ 이미 연결이 있으면 재사용, 없으면 새로 생성
    if (QSqlDatabase::contains("venti_connection")) {
        db = QSqlDatabase::database("venti_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "venti_connection");
        db.setDatabaseName(dbPath);
    }

    qDebug() << "현재 사용 중인 DB 절대 경로:" << dbPath;
}

// ✅ initDatabase: addDatabase 호출 없이 그냥 열기만
bool DatabaseManager::initDatabase(const QString& dbName) {
    // dbName이 절대경로가 아닌 경우 대비
    if (!dbName.isEmpty()) {
        db.setDatabaseName(dbName);
    }

    if (!db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
        return false;
    }

    qDebug() << "DB 연결 성공!";

    QSqlQuery query(db);
    query.exec("PRAGMA foreign_keys = ON;");

    return true;
}

// ✅ setupDatabase: 테이블 생성만 담당, insertInitialData 호출 안 함
bool DatabaseManager::setupDatabase() {
    if (!db.isOpen() && !db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.exec("PRAGMA foreign_keys = ON;");

    query.exec("CREATE TABLE IF NOT EXISTS CATEGORIES (category_id INTEGER PRIMARY KEY, category_name VARCHAR(50));");
    query.exec("CREATE TABLE IF NOT EXISTS MEMBERS (phone_num VARCHAR(20) PRIMARY KEY, stamp_count INTEGER, total_pay_amount INTEGER);");
    query.exec("CREATE TABLE IF NOT EXISTS MENU_INFO (menu_id INTEGER PRIMARY KEY, category_id INTEGER, kr_name VARCHAR(100), price INTEGER, size_info VARCHAR(50), product_type VARCHAR(20), is_soldout INTEGER, image_path VARCHAR(255), etc_info TEXT, FOREIGN KEY (category_id) REFERENCES CATEGORIES(category_id));");
    query.exec("CREATE TABLE IF NOT EXISTS ORDERS (order_id INTEGER PRIMARY KEY, member_phone VARCHAR(20), order_date DATETIME, total_amount INTEGER, pay_method VARCHAR(30), is_takeout INTEGER, FOREIGN KEY (member_phone) REFERENCES MEMBERS(phone_num));");
    query.exec("CREATE TABLE IF NOT EXISTS ORDER_ITEMS (item_id INTEGER PRIMARY KEY, order_id INTEGER, menu_id INTEGER, quantity INTEGER, subtotal INTEGER, selected_options TEXT, FOREIGN KEY (order_id) REFERENCES ORDERS(order_id), FOREIGN KEY (menu_id) REFERENCES MENU_INFO(menu_id));");

    qDebug() << "DB 테이블 세팅 완료";

    // ✅ 여기서 딱 한 번만 호출
    insertInitialData();

    return true;
}

bool DatabaseManager::insertInitialData() {
    QStringList categories;
    QList<MenuData> menus;

    if (!MenuDataLoader::loadFromJson(":/data/menus.json", categories, menus)) {
        qDebug() << "데이터 파일을 읽을 수 없습니다.";
        return false;
    }

    QSqlQuery query(db);  // ✅ db 연결 명시

    for (const QString &catName : categories) {
        query.prepare("INSERT INTO CATEGORIES (category_name) "
                      "SELECT :name WHERE NOT EXISTS (SELECT 1 FROM CATEGORIES WHERE category_name = :name)");
        query.bindValue(":name", catName);
        if (!query.exec()) {
            qDebug() << "카테고리 삽입 실패:" << query.lastError().text();
        }
    }

    for (const auto &m : menus) {
        query.prepare("INSERT INTO MENU_INFO (category_id, kr_name, price, image_path, is_soldout) "
                      "SELECT (SELECT category_id FROM CATEGORIES WHERE category_name = :cat), "
                      ":name, :price, :img, 0 "
                      "WHERE NOT EXISTS (SELECT 1 FROM MENU_INFO WHERE kr_name = :name)");
        query.bindValue(":cat", m.category);
        query.bindValue(":name", m.name);
        query.bindValue(":price", m.price);
        query.bindValue(":img", m.imgPath);

        if (!query.exec()) {
            qDebug() << "메뉴 삽입 실패 (" << m.name << "): " << query.lastError().text();
        }
    }

    QSqlQuery testQuery("SELECT COUNT(*) FROM MENU_INFO", db);
    if (testQuery.next()) {
        qDebug() << "검증 - MENU_INFO 테이블 레코드 수:" << testQuery.value(0).toInt();
    }

    qDebug() << "JSON 데이터를 기반으로 DB 구축이 완료되었습니다.";
    return true;
}

QStringList DatabaseManager::getCategoryNames() {
    QStringList list;
    QSqlQuery query("SELECT category_name FROM CATEGORIES ORDER BY category_id", db);

    while (query.next()) {
        list.append(query.value(0).toString());
    }
    return list;
}

QList<QVariantMap> DatabaseManager::getMenusByCategory(const QString &categoryName) {
    QList<QVariantMap> menuList;
    QSqlQuery query(db);  // ✅ db 연결 명시

    query.prepare("SELECT kr_name, price, image_path FROM MENU_INFO "
                  "WHERE category_id = (SELECT category_id FROM CATEGORIES WHERE category_name = :catName)");
    query.bindValue(":catName", categoryName);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap menu;
            menu["name"] = query.value(0).toString();
            menu["price"] = query.value(1).toInt();
            menu["image"] = query.value(2).toString();
            menuList.append(menu);
        }
    } else {
        qDebug() << "메뉴 조회 실패:" << query.lastError().text();
    }
    return menuList;
}
