#ifndef DATABASE_H
#define DATABASE_H

#include <memory>

enum class TypeDB { sqlite, postgres, unknown };

namespace SQLite {
class Database;
};

namespace YAML {
class Node;
};

TypeDB hashString(const std::string &typeDB);
std::unique_ptr<SQLite::Database> NewDBClient();

#endif
