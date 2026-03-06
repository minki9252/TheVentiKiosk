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

    QStringList categories = {"신메뉴", "커피", "버블티&티", "블렌디드", "베버리지", "에이드"};
    for (const QString &catName : categories) {
        query.prepare("INSERT INTO CATEGORIES (category_name) "
                      "SELECT :name WHERE NOT EXISTS (SELECT 1 FROM CATEGORIES WHERE category_name = :name)");
        query.bindValue(":name", catName);
        query.exec();
    }


    struct MenuData {
        QString category;
        QString name;
        int price;
        QString imgPath;
        MenuData(QString c, QString n, int p, QString i) : category(c), name(n), price(p), imgPath(i) {}
    };

    QList<MenuData> menus;

    // 신메뉴
    menus.append(MenuData("신메뉴", "꿀배생강티", 3500, ":/New_menu/honey_pear.png"));
    menus.append(MenuData("신메뉴", "더 쌍화차", 3500, ":/New_menu/ssanghwa.png"));
    menus.append(MenuData("신메뉴", "오미자석류티", 3500, ":/New_menu/omija.png"));

    // 커피
    menus.append(MenuData("커피", "아메리카노", 2000, ":/Coffee/americano.png"));
    menus.append(MenuData("커피", "믹스커피", 2500, ":/Coffee/mix.png"));
    menus.append(MenuData("커피", "아인슈페너", 3500, ":/Coffee/einspanner.png"));
    menus.append(MenuData("커피", "카라멜마끼아또", 3500, ":/Coffee/caramel.png"));
    menus.append(MenuData("커피", "카페라떼", 3000, ":/Coffee/latte.png"));
    menus.append(MenuData("커피", "카페모카", 3500, ":/Coffee/mocha.png"));
    menus.append(MenuData("커피", "토피넛라떼", 3700, ":/Coffee/toffeenut.png"));

    // 버블티&티
    menus.append(MenuData("버블티&티", "리치캐모마일티", 2500, ":/Bubble_tea/rich_camomile.png"));
    menus.append(MenuData("버블티&티", "복숭아아이스티", 3000, ":/Bubble_tea/peach_ice_tea.png"));
    menus.append(MenuData("버블티&티", "자몽허니블랙티", 3500, ":/Bubble_tea/grapefruit_honey.png"));
    menus.append(MenuData("버블티&티", "타로버블티", 3900, ":/Bubble_tea/taro_bubble.png"));
    menus.append(MenuData("버블티&티", "흑설탕버블티", 3900, ":/Bubble_tea/black_sugar_bubble.png"));

    // 블렌디드
    menus.append(MenuData("블렌디드", "드래곤스무디소다", 4500, ":/Blended/dragon_soda.png"));
    menus.append(MenuData("블렌디드", "드래곤스무디요거트", 4300, ":/Blended/dragon_yogurt.png"));
    menus.append(MenuData("블렌디드", "딸기요거트스무디", 3900, ":/Blended/strawberry_yogurt.png"));
    menus.append(MenuData("블렌디드", "망고요거트스무디", 3900, ":/Blended/mango_yogurt.png"));
    menus.append(MenuData("블렌디드", "밀크쉐이크", 3900, ":/Blended/milk_shake.png"));
    menus.append(MenuData("블렌디드", "블루베리요거트스무디", 3900, ":/Blended/blueberry_yogurt.png"));
    menus.append(MenuData("블렌디드", "자바칩프라페", 3900, ":/Blended/javachip.png"));
    menus.append(MenuData("블렌디드", "초코쉐이크", 3900, ":/Blended/choco_shake.png"));
    menus.append(MenuData("블렌디드", "쿠키앤크림프라페", 3900, ":/Blended/cookie_cream.png"));
    menus.append(MenuData("블렌디드", "플레인요거트스무디", 3900, ":/Blended/plain_yogurt.png"));

    // 베버리지
    menus.append(MenuData("베버리지", "군고구마라떼", 3500, ":/Beverage/sweet_potato.png"));
    menus.append(MenuData("베버리지", "딸기라떼", 3500, ":/Beverage/strawberry_latte.png"));
    menus.append(MenuData("베버리지", "로얄밀크티", 3500, ":/Beverage/royal_milk_tea.png"));
    menus.append(MenuData("베버리지", "말차라떼", 3500, ":/Beverage/matcha_latte.png"));
    menus.append(MenuData("베버리지", "말차아인슈페너", 3500, ":/Beverage/matcha_einspanner.png"));
    menus.append(MenuData("베버리지", "미숫가루", 3000, ":/Beverage/misugaru.png"));

    // 에이드
    menus.append(MenuData("에이드", "레몬에이드", 3900, ":/Ade/lemon_ade.png"));
    menus.append(MenuData("에이드", "애플망고에이드", 3900, ":/Ade/apple_mango.png"));
    menus.append(MenuData("에이드", "자몽에이드", 3900, ":/Ade/grapefruit_ade.png"));

    // 데이터 삽입 실행
    for (const auto &m : menus) {
        query.prepare("INSERT INTO MENU_INFO (category_id, kr_name, price, image_path, is_soldout) "
                      "SELECT (SELECT category_id FROM CATEGORIES WHERE category_name = :cat), :name, :price, :img, 0 "
                      "WHERE NOT EXISTS (SELECT 1 FROM MENU_INFO WHERE kr_name = :name)");
        query.bindValue(":cat", m.category);
        query.bindValue(":name", m.name);
        query.bindValue(":price", m.price);
        query.bindValue(":img", m.imgPath);
        query.exec();
    }
    qDebug() << "더벤티 메뉴 DB 구축 완료!";
}
