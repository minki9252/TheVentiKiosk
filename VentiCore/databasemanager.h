#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStringList>

class DatabaseManager {
public:
    // 싱글톤 패턴이나 전역 인스턴스로 관리하면 편리
    static DatabaseManager& instance();

    bool initDatabase(const QString& dbName);
    bool setupDatabase();      // DB 연결 및 테이블 생성
    void insertInitialData();  // 초기 메뉴 데이터 삽입

private:
    DatabaseManager(); // 생성자 비공개
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H