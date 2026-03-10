#include "databasemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>

// 데이터를 읽어오는 객체
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


void initDatabase() {
    // SQLite DB 연결 설정
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("venti.db");
}


// static으로 선언된 instance 함수 구현
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}


bool DatabaseManager::initDatabase(const QString& dbName) {

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (db.open()) {
        setupDatabase();      // 테이블 생성
        insertInitialData();  // 데이터 삽입
        return true;
    }
    return false;

    qDebug() << "DB 연결 성공!";

    // 외래 키 활성화 및 테이블 생성 로직
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;");

    // 테이블 생성
    if(setupDatabase()) { 
        insertInitialData(); 
    }
    
    return true;
}


DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QCoreApplication::applicationDirPath() + "/venti.db";
    db.setDatabaseName(dbPath);

    qDebug() << "현재 사용 중인 DB 절대 경로:" << dbPath;
}

bool DatabaseManager::setupDatabase() {
    if (!db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;");    // FK 활성화

    // 테이블 생성 쿼리
    query.exec("CREATE TABLE CATEGORIES (category_id INTEGER PRIMARY KEY, category_name VARCHAR(50));");
    query.exec("CREATE TABLE MEMBERS (phone_num VARCHAR(20) PRIMARY KEY, stamp_count INTEGER, total_pay_amount INTEGER);");
    query.exec("CREATE TABLE MENU_INFO (menu_id INTEGER PRIMARY KEY, category_id INTEGER, kr_name VARCHAR(100), price INTEGER, size_info VARCHAR(50), product_type VARCHAR(20), is_soldout INTEGER, image_path VARCHAR(255), etc_info TEXT, FOREIGN KEY (category_id) REFERENCES CATEGORIES(category_id));");
    query.exec("CREATE TABLE ORDERS (order_id INTEGER PRIMARY KEY, member_phone VARCHAR(20), order_date DATETIME, total_amount INTEGER, pay_method VARCHAR(30), is_takeout INTEGER, FOREIGN KEY (member_phone) REFERENCES MEMBERS(phone_num));");
    query.exec("CREATE TABLE ORDER_ITEMS (item_id INTEGER PRIMARY KEY, order_id INTEGER, menu_id INTEGER, quantity INTEGER, subtotal INTEGER, selected_options TEXT, FOREIGN KEY (order_id) REFERENCES ORDERS(order_id), FOREIGN KEY (menu_id) REFERENCES MENU_INFO(menu_id));");
    qDebug() << "DB 테이블 세팅 완료";
    
    // 테이블 생성 호출
    insertInitialData();
    
    return true;
}

bool DatabaseManager::insertInitialData() {
    QSqlQuery query;

    // 확실하게 기존 데이터를 모두 삭제합
    if(!query.exec("DELETE FROM MENU_INFO")) qDebug() << "MENU_INFO 삭제 실패";
    if(!query.exec("DELETE FROM CATEGORIES")) qDebug() << "CATEGORIES 삭제 실패";

    query.exec("DELETE FROM sqlite_sequence WHERE name='CATEGORIES' OR name='MENU_INFO'");

    QStringList categories;
    QList<MenuData> menus;

    // JSON 로드
    if (!MenuDataLoader::loadFromJson(":/data/menus.json", categories, menus)) {
        return false;
    }

    // 카테고리 삽입 부분 수정
    for (const QString &cat : categories) {
        query.prepare("INSERT INTO CATEGORIES (category_name) VALUES (:name)");
        query.bindValue(":name", cat.trimmed());
        query.exec();
    }

    // 메뉴 정보 삽입 부분 수정
    for (const auto &m : menus) {
        query.prepare("INSERT INTO MENU_INFO (category_id, kr_name, price, image_path, is_soldout) "
                      "SELECT (SELECT category_id FROM CATEGORIES WHERE category_name = :cat), "
                      ":name, :price, :img, 0");

        query.bindValue(":cat", m.category.trimmed());
        query.bindValue(":name", m.name.trimmed());
        query.bindValue(":price", m.price);
        query.bindValue(":img", m.imgPath.trimmed());

        query.exec();
    }

    qDebug() << "📢 DB 초기화 및 데이터 삽입 완료";
    return true;
}

bool DatabaseManager::updateMenuImagePath(const QString &menuName, const QString &newPath) {
    QSqlQuery query;

    query.prepare("UPDATE MENU_INFO SET image_path = :path WHERE kr_name = :name");
    query.bindValue(":path", newPath);
    query.bindValue(":name", menuName);

    if (!query.exec()) {
        qDebug() << "경로 업데이트 실패:" << query.lastError().text();
        return false;
    }
    return true;
}

// 카테고리 목록 호출 함수
QStringList DatabaseManager::getCategoryNames() {
    QStringList list;
    QSqlQuery query("SELECT category_name FROM CATEGORIES ORDER BY category_id");

    while (query.next()) {
        // DB에서 꺼낸 카테고리 이름을 리스트에 담기
        list.append(query.value(0).toString());
    }
    return list;
}

// 특정 카테고리의 메뉴 호출
QList<QVariantMap> DatabaseManager::getMenusByCategory(const QString &categoryName) {
    QList<QVariantMap> menuList;
    QSqlQuery query;

    // 조인을 사용해 카테고리 이름으로 해당 메뉴들을 한 번에 찾기
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
    }
    return menuList;
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
