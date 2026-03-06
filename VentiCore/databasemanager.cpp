#include "databasemanager.h"

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

bool DatabaseManager::initDatabase(const QString& dbName) {

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
        return false;
    }
    
    qDebug() << "DB 연결 성공!";
    
    // 외래 키 활성화 및 테이블 생성 로직
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;");
    
    // 필요한 경우 여기서 createTables() 같은 함수를 추가로 호출
    return true;
}

DatabaseManager::DatabaseManager() {
    // 생성자에서 DB 객체 초기화
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("venti.db");
}

bool DatabaseManager::setupDatabase() {
    if (!db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;"); // 외래키 활성화

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

void DatabaseManager::insertInitialData() {
    QSqlQuery query;

    QStringList categories = {"New_menu", "Coffe", "Bubble_tea", "Blended", "Beverage", "Ade"};
    
    for (const QString &catName : categories) {
        query.prepare("INSERT INTO CATEGORIES (category_name) "
                      "SELECT :name WHERE NOT EXISTS (SELECT 1 FROM CATEGORIES WHERE category_name = :name)");
        query.bindValue(":name", catName);
        if (!query.exec()) qDebug() << "카테고리 삽입 실패:" << query.lastError().text();
    }

    // 2. 초기 메뉴 데이터 삽입
    struct MenuData {
        int catId;
        QString name;
        int price;
        QString img;
    };

    // 예시 데이터 (팀원들이 바로 사용할 수 있게 넉넉히 넣어주세요)
    QList<MenuData> items = {
        {3, "아이스 아메리카노", 1800, ":/Coffe/ice_americano.png"},
        {3, "아이스 카페라떼", 2700, ":/Coffe/ice_latte.png"},
        {4, "핫 아메리카노", 1500, ":/Coffe/hot_americano.png"},
        {1, "GD 추천 신메뉴", 4500, ":/G-dragon/gd_drink.png"} // G-dragon 폴더 참고
    };

    for (const auto &item : items) {
        query.prepare("INSERT INTO MENU_INFO (category_id, kr_name, price, image_path, is_soldout) "
                      "SELECT :cat, :name, :price, :img, 0 "
                      "WHERE NOT EXISTS (SELECT 1 FROM MENU_INFO WHERE kr_name = :name)");
        query.bindValue(":cat", item.catId);
        query.bindValue(":name", item.name);
        query.bindValue(":price", item.price);
        query.bindValue(":img", item.img);
        query.exec();
    }

    qDebug() << "더벤티 초기 데이터(메뉴/카테고리) 로드 완료!";
}
