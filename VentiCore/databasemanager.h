#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "VentiCore_global.h"
#include <QSqlDatabase>
#include <QSqlQuery>

// DB 관리 클래스
class VENTICORE_EXPORT DatabaseManager {
public:
    static DatabaseManager& instance();
    bool initDatabase(const QString& dbPath);
    void createTables();
    QSqlQuery executeQuery(const QString& queryStr);

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};

#endif
