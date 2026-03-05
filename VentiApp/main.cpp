#include "mainwindow.h"
#include <QApplication>
#include "databasemanager.h" // 라이브러리 헤더 포함

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // DB 초기화
    if (DatabaseManager::instance().initDatabase("theventi.db")) {
    }

    MainWindow w;
    w.show();
    return a.exec();
}
