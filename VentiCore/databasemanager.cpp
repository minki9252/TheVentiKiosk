#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>


void initDatabase() {
    // SQLite DB 연결 설정
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("theventi.db");

    if (!db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
    } else {
        qDebug() << "DB 연결 성공!";
        // 여기서 필요한 테이블(메뉴, 주문 등) 생성 쿼리를 실행할 수 있습니다.
    }
}

// static으로 선언된 instance 함수 구현
DatabaseManager& DatabaseManager::instance() {
    // 프로그램 실행 시 단 한 번만 생성되는 정적 객체
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {
    // SQLite 드라이버를 사용하도록 설정
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}


bool DatabaseManager::initDatabase(const QString& dbPath) {
    m_db.setDatabaseName(dbPath);

    // DB 파일 열기 시도 및 예외 처리
    if (!m_db.open()) {
        qDebug() << "DB 연결 실패:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "DB 연결 성공:" << dbPath;
    return true;
}

QSqlQuery DatabaseManager::executeQuery(const QString& queryStr) {
    QSqlQuery query;
    // 쿼리 실행 후 에러가 나면 콘솔에 출력 (예외 처리)
    if (!query.exec(queryStr)) {
        qDebug() << "SQL 에러:" << query.lastError().text();
    }
    return query;
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

// DB 테이블을 생성하고 초기 데이터를 넣는 함수 (A파트 팀장 담당)
void DatabaseManager::createTables() {
    QSqlQuery query;

    // 1. 카테고리 테이블 (커피, 에이드 등)
    query.exec("CREATE TABLE IF NOT EXISTS categories ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL)");

    // 2. 메뉴 테이블
    query.exec("CREATE TABLE IF NOT EXISTS menus ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "category_id INTEGER, "
               "name TEXT NOT NULL, "
               "price INTEGER, "
               "image_path TEXT, "
               "FOREIGN KEY(category_id) REFERENCES categories(id))");

    // [초기 데이터 주입] 데이터가 없을 때만 넣기
    query.exec("SELECT COUNT(*) FROM categories");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO categories (name) VALUES ('커피'), ('에이드'), ('버블티')");
        query.exec("INSERT INTO menus (category_id, name, price) VALUES (1, '바닐라라떼', 3500)");
        query.exec("INSERT INTO menus (category_id, name, price) VALUES (1, '아이스 아메리카노', 2000)");
        qDebug() << "초기 샘플 데이터 주입 완료!";
    }
}
