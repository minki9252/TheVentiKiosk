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
        // 주석: 리소스 경로에서 파일을 읽기 전용으로 엽니다.
        if (!file.open(QIODevice::ReadOnly)) return false;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject root = doc.object();

        // 주석: 카테고리 목록을 동적으로 추출합니다.
        QJsonArray catArray = root["categories"].toArray();
        for (const QJsonValue &v : catArray) categories << v.toString();

        // 주석: 메뉴 상세 정보를 객체 리스트에 담습니다.
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

    if (!db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
    } else {
        qDebug() << "DB 연결 성공!";
        // 여기서 필요한 테이블(메뉴, 주문 등) 생성 쿼리를 실행할 수 있음
    }
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
    // 1. 외부 파일에서 데이터 로드 (객체화 및 하드코딩 분리)
    QStringList categories;
    QList<MenuData> menus;

    // JSON 로더를 통해 데이터를 객체 리스트로 가져옴
    if (!MenuDataLoader::loadFromJson(":/data/menus.json", categories, menus)) {
        qDebug() << "데이터 파일을 읽을 수 없습니다.";
        return false;
    }

    QSqlQuery query;

    // 2. 카테고리 자동 삽입 (중복 방지 로직 포함)
    for (const QString &catName : categories) {
        // 주석: 카테고리 이름이 없을 때만 삽입하여 데이터 무결성 유지
        query.prepare("INSERT INTO CATEGORIES (category_name) "
                      "SELECT :name WHERE NOT EXISTS (SELECT 1 FROM CATEGORIES WHERE category_name = :name)");
        query.bindValue(":name", catName);

        if (!query.exec()) {
            qDebug() << "카테고리 삽입 실패:" << query.lastError().text();
        }
    }

    // 3. 메뉴 정보 자동 삽입 (JOIN 서브쿼리 활용)
    // 주석: MenuData 객체의 멤버 변수들을 쿼리에 바인딩
    for (const auto &m : menus) {
        query.prepare("INSERT INTO MENU_INFO (category_id, kr_name, price, image_path, is_soldout) "
                      "SELECT (SELECT category_id FROM CATEGORIES WHERE category_name = :cat), "
                      ":name, :price, :img, 0 "
                      "WHERE NOT EXISTS (SELECT 1 FROM MENU_INFO WHERE kr_name = :name)");

        // 데이터 바인딩 위치 지정
        query.bindValue(":cat", m.category);    // 카테고리 이름으로 ID 조회 후 삽입
        query.bindValue(":name", m.name);       // 메뉴 한글명
        query.bindValue(":price", m.price);     // 가격 (정수형)
        query.bindValue(":img", m.imgPath);     // 이미지 경로 (qrc 경로)

        if (!query.exec()) {
            qDebug() << "메뉴 삽입 실패 (" << m.name << "): " << query.lastError().text();
        }
    }

    QSqlQuery testQuery("SELECT COUNT(*) FROM MENU_INFO");
    if (testQuery.next()) {
        qDebug() << "검증 - MENU_INFO 테이블 레코드 수:" << testQuery.value(0).toInt();
    }

    qDebug() << "JSON 데이터를 기반으로 DB 구축이 완료되었습니다.";
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

    // 조인(JOIN)을 사용해 카테고리 이름으로 해당 메뉴들을 한 번에 찾기
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
