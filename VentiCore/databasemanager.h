#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "VentiCore_global.h"   // 라이브러리 내보내기용
#include <QSqlDatabase>
#include <QSqlQuery>

// DB 관리 클래스
// VENTICORESHARED_EXPORT가 붙어야 외부(VentiApp)에서 이 클래스를 인식
class VENTICORE_EXPORT DatabaseManager {
public:
    // 싱글톤 인스턴스 반환 함수
    static DatabaseManager& instance();
    // DB 연결 초기화(성공시 true)
    bool initDatabase(const QString& dbPath);
    // 함수 등록
    void createTables();
    // 공통 쿼리 실행
    QSqlQuery executeQuery(const QString& queryStr);

private:
    DatabaseManager();  // 외부 생성 방지
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};

#endif
