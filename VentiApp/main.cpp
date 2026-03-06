#include "mainwindow.h"
#include <QApplication>
#include "databasemanager.h" // 라이브러리 헤더 포함
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // DatabaseManager 라이브러리의 함수를 호출
    if (!DatabaseManager::instance().initDatabase("venti.db")) {
        qDebug() << "시스템을 종료합니다. DB 설정을 확인하세요.";
    }

    MainWindow w;
    w.show();
    
    return a.exec();
}
