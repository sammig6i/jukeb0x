#ifndef DATABASE_H
#define DATABASE_H

#include <SQLiteCpp/Database.h>
#include <yaml-cpp/node/node.h>

std::unique_ptr<SQLite::Database>
NewSQLiteClient(const YAML::Node &config,
                int flags = SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

void CreateTables(std::unique_ptr<SQLite::Database> &db);
std::string GetDBPath(const YAML::Node &config);

#endif // !DATABASE_H
