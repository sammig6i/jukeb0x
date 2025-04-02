#ifndef SQLITE_H
#define SQLITE_H

#include <memory>
#include <stdlib.h>

namespace SQLite {
class Database;
};

std::unique_ptr<SQLite::Database>
NewSQLiteClient(const std::string &dataSourceName);
void CreateTables(std::unique_ptr<SQLite::Database> &db);

#endif
