#include "mainwindow.h"
#include <QApplication>
#include "databasemanager.h" // 라이브러리 헤더 포함

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. DB 초기화
    if (DatabaseManager::instance().initDatabase("theventi.db")) {
        // 테이블 생성 로직 호출 (필요시 내부에서 호출하도록 설계 가능)
        // DatabaseManager::instance().createTables();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
