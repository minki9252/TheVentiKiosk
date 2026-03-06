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
    bool insertInitialData();  // 초기 메뉴 데이터 삽입

    // 카테고리 이름 목록 가져오기(3번 화면 상단)
    QStringList getCategoryNames();

    // 특정 카테고리에 속한 메뉴 정보 가져오기 (3번 화면 중간))
    QList<QVariantMap> getMenusByCategory(const QString &categoryName);

private:
    DatabaseManager(); // 생성자 비공개
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
