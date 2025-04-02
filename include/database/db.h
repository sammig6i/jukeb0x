#ifndef DATABASE_H
#define DATABASE_H

#include <memory>

// TODO: update with postgres for future
enum class TypeDB { sqlite, postgres, unknown };

namespace SQLite {
class Database;
};

TypeDB hashString(const std::string &typeDB);
std::unique_ptr<SQLite::Database> NewDBClient();

#endif
