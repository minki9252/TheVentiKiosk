#include "mainwindow.h"
#include <QApplication>
#include "databasemanager.h" // 라이브러리 헤더 포함
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. DB 초기화
    if (DatabaseManager::instance().initDatabase("venti.db")) {
        // 테이블 생성 로직 호출 (필요시 내부에서 호출하도록 설계 가능)
        // DatabaseManager::instance().createTables();
    }

    MainWindow w;
    w.show();
    return a.exec();
}

void initDatabase() {
    // SQLite DB 연결 설정
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("venti.db");

    if (!db.open()) {
        qDebug() << "DB 연결 실패:" << db.lastError().text();
    } else {
        qDebug() << "DB 연결 성공!";
        // 여기서 필요한 테이블(메뉴, 주문 등) 생성 쿼리를 실행할 수 있습니다.
    }
}
